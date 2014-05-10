#include "ros/ros.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/TwistStamped.h"
#include "simulator_physics_model/auv_model.hpp"
#include "geometry_msgs/Transform.h"
#include "tf/transform_datatypes.h"
#include "sensor_msgs/Imu.h"
#include "nav_msgs/Odometry.h"

using namespace ros;
using namespace ikat_simulator;

float storeforce[]={0.0,0.0,0,0,-0.075,-0.075};
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

public:
    Functor(NodeHandle&n,AuvModelSimple6DoF&a,Publisher &pub,Publisher &t,Publisher &odo,Publisher &imu,int type)
        :nodeh(n),auv(a),pose_pub(pub),twist_pub(t),odo_pub(odo),imu_pub(imu),isLineNeeded(type){}


    void operator()(const TimerEvent& t)
    {
        geometry_msgs::Pose pos_msg;
        pos_msg.position.x=auv._current_position_to_body[0];

        pos_msg.position.y=auv._current_position_to_body[1];
        pos_msg.position.z=auv._current_position_to_body[2];
        tf::Quaternion quat = tf::createQuaternionFromRPY(auv._current_position_to_body[3],auv._current_position_to_body[4],auv._current_position_to_body[5]);
//        pos_msg.orientation= quat;
        pos_msg.orientation.x=quat.getX ();
        pos_msg.orientation.y=quat.getY ();
        pos_msg.orientation.z=quat.getZ ();
        pos_msg.orientation.w=quat.getW ();
        //ROS_INFO("%f %f %f ",pos_msg.position.x,pos_msg.position.y,pos_msg.position.z);
        pose_pub.publish(pos_msg);

        geometry_msgs::TwistStamped twist_msg;
        twist_msg.twist.linear.x=auv._current_velocity_state_to_body[0];
        twist_msg.twist.linear.y=auv._current_velocity_state_to_body[1];
        twist_msg.twist.linear.z=auv._current_velocity_state_to_body[2];
        twist_msg.twist.angular.x=auv._current_velocity_state_to_body[3];
        twist_msg.twist.angular.y=auv._current_velocity_state_to_body[4];
        twist_msg.twist.angular.z=auv._current_velocity_state_to_body[5];
        twist_msg.header.frame_id='1';
        twist_pub.publish(twist_msg);

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




int main(int argc,char **argv)
{
    ros::init (argc,argv,"simulator_node_1");
    int type=0;
    if(argv[1]!=NULL)
        type=atoi(argv[1]);
    NodeHandle n;

    Publisher pose_publisher=n.advertise<geometry_msgs::Pose>("/kraken/pose",100);
    Publisher twistS_publisher= n.advertise <geometry_msgs::TwistStamped>("/kraken/twist",100);
    Publisher odometry_pub= n.advertise <nav_msgs::Odometry>("/kraken/dataNavigator",100);
    Publisher imu_pub=n.advertise<sensor_msgs::Imu>("/kraken/imu_data",100);


    AuvModelSimple6DoF auv(0.08);

    Functor timerCallback(n,auv,pose_publisher,twistS_publisher,odometry_pub,imu_pub,type);
    Timer timer=n.createTimer(ros::Duration(0.08),timerCallback);
    timer.start ();


    spin ();

    return 0;

}
