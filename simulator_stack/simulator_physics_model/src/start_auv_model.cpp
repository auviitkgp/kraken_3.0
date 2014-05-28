#include "ros/ros.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/TwistStamped.h"
#include "simulator_physics_model/auv_model.hpp"
#include "geometry_msgs/Transform.h"
#include "tf/transform_datatypes.h"
#include "sensor_msgs/Imu.h"
#include "nav_msgs/Odometry.h"
#include "kraken_msgs/thrusterData4Thruster.h"
#include "kraken_msgs/thrusterData6Thruster.h"


using namespace ros;
using namespace kraken_simulator;



float storeforce[]={1.00,1.00,1,-1,-0.00 + 0.10,-0.10 + 0.00};
void updateAUV(AuvModelSimple6DoF& auv,float force[])
{
    for(int i=0;i<6;i++)storeforce[i]=force[i];
    auv.updateAuv (force);
}


class Functor{
    NodeHandle &nodeh;
    AuvModelSimple6DoF& auv;
    Publisher &pose_pub,&twist_pub,odo_pub,&imu_pub;
    int isLineNeeded;
    int count;

public:
    Functor(NodeHandle&n,AuvModelSimple6DoF&a,Publisher &pub,Publisher &t,Publisher &odo,Publisher &imu,int val=0)
        :nodeh(n),auv(a),pose_pub(pub),twist_pub(t),odo_pub(odo),imu_pub(imu),isLineNeeded(val),count(0){}


    void operator()(const TimerEvent& t)
    {
        geometry_msgs::Pose pos_msg;
        pos_msg.position.x=auv._current_position_to_world[0];
        pos_msg.position.y=auv._current_position_to_world[1];
        pos_msg.position.z=auv._current_position_to_world[2];
        //tf::Quaternion quat = tf::createQuaternionFromRPY(auv._current_position_to_world[3],auv._current_position_to_world[4],auv._current_position_to_world[5]);
        tf::Quaternion quat = tf::createQuaternionFromRPY(auv._current_position_to_body[3],auv._current_position_to_body[4],auv._current_position_to_body[5]);
        //tf::Quaternion quat = tf::createQuaternionFromRPY(/*auv._current_position_to_body[3]*/0,/*auv._current_position_to_body[4]*/0,/*auv._current_position_to_body[5]*/0.78);
//        pos_msg.orientation= quat;
        pos_msg.orientation.x=quat.getX ();
        pos_msg.orientation.y=quat.getY ();
        pos_msg.orientation.z=quat.getZ ();
        pos_msg.orientation.w=quat.getW ();
        //ROS_INFO("%f %f %f ",pos_msg.position.x,pos_msg.position.y,pos_msg.position.z);
        pose_pub.publish(pos_msg);
        /*if(count ==100)
        {
            storeforce[2]=0;
            storeforce[3]=0;
            storeforce[4]=0;
            storeforce[5]=0;
        }
        else
        {
            count++;
        }
        */
        geometry_msgs::TwistStamped twist_msg;
        twist_msg.twist.linear.x=auv._current_velocity_state_to_body[0];
        twist_msg.twist.linear.y=auv._current_velocity_state_to_body[1];
        twist_msg.twist.linear.z=auv._current_velocity_state_to_body[2];
        twist_msg.twist.angular.x=auv._current_velocity_state_to_body[3];
        twist_msg.twist.angular.y=auv._current_velocity_state_to_body[4];
        twist_msg.twist.angular.z=auv._current_velocity_state_to_body[5];
        twist_msg.header.frame_id='1';
        //twist_pub.publish(twist_msg);

        nav_msgs::Odometry odo_msg;
        odo_msg.pose.pose=pos_msg;
        odo_msg.twist.twist=twist_msg.twist;
        if(isLineNeeded)
            odo_pub.publish(odo_msg);

        sensor_msgs::Imu imu_msg;
        imu_msg.angular_velocity=twist_msg.twist.angular;
        imu_msg.linear_acceleration.x=auv._current_accelaration_to_body[0];
        imu_msg.linear_acceleration.y=auv._current_accelaration_to_body[1];
        imu_msg.linear_acceleration.z=auv._current_accelaration_to_body[2];
        imu_msg.orientation=pos_msg.orientation;
        imu_pub.publish(imu_msg);



        auv.updateAuv (storeforce);
    }
};

void fourThrustCb(const kraken_msgs::thrusterData4ThrusterConstPtr &msg)
{
    storeforce[0]=msg->data[0];
    storeforce[1]=msg->data[1];
    storeforce[2]=0;
    storeforce[3]=0;
    storeforce[4]=msg->data[2];
    storeforce[5]=msg->data[3];
}

void sixThrustCb(const kraken_msgs::thrusterData6ThrusterConstPtr &msg)
{
    storeforce[0]=msg->data[0];
    storeforce[1]=msg->data[1];
    storeforce[2]=msg->data[2];
    storeforce[3]=msg->data[3];
    storeforce[4]=msg->data[4];
    storeforce[5]=msg->data[5];
}




int main(int argc,char **argv)
{
    ros::init (argc,argv,"simulator_node_1");
    int type=0;
    if(argc>=8)
    {
        type=atoi(argv[7]);

    }
    for(int i=1;i<argc&&i<7;i++)
            storeforce[i-1]=atof(argv[i]);
    NodeHandle n;

    Publisher pose_publisher=n.advertise<geometry_msgs::Pose>("/kraken/pose",100);
    Publisher twistS_publisher= n.advertise <geometry_msgs::TwistStamped>("/kraken/twist",100);
    Publisher odometry_pub= n.advertise <nav_msgs::Odometry>("/kraken/dataNavigator",100);
    Publisher imu_pub=n.advertise<sensor_msgs::Imu>("/kraken/imu_data",100);

    Subscriber thrust4sub=n.subscribe<kraken_msgs::thrusterData4Thruster>("/kraken/forceData4Thruster",100,fourThrustCb);
    Subscriber thrust6sub=n.subscribe<kraken_msgs::thrusterData6Thruster>("/kraken/forceData6Thruster",100,sixThrustCb);

    AuvModelSimple6DoF auv(0.1);

    Functor timerCallback(n,auv,pose_publisher,twistS_publisher,odometry_pub,imu_pub,type);
    Timer timer=n.createTimer(ros::Duration(0.1),timerCallback);
    timer.start ();

    spin ();

    return 0;

}
