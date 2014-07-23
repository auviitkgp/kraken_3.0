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
#include <kraken_msgs/forceData4Thruster.h>
#include <kraken_msgs/forceData6Thruster.h>
#include <kraken_msgs/imuData.h>
#include <tracks_imu/Tracks.h>
#include "resources/topicHeader.h"

using namespace ros;
using namespace kraken_simulator;



float storeforce[]={0,0,0,0,0,0};
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
        auv.updateAuv (storeforce);
        geometry_msgs::Pose pos_msg;
        kraken_msgs::imuData imu;
        /*************************************/
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
        /*************************************/
        geometry_msgs::TwistStamped twist_msg;
        twist_msg.twist.linear.x=auv._current_velocity_state_to_body[0];
        twist_msg.twist.linear.y=auv._current_velocity_state_to_body[1];
        twist_msg.twist.linear.z=auv._current_velocity_state_to_body[2];
        imu.data[kraken_sensors::gyroX] = twist_msg.twist.angular.x=auv._current_velocity_state_to_body[3];
        imu.data[kraken_sensors::gyroY]= twist_msg.twist.angular.y=auv._current_velocity_state_to_body[4];
        imu.data[kraken_sensors::gyroZ]  = twist_msg.twist.angular.z=auv._current_velocity_state_to_body[5];
        twist_msg.header.frame_id='1';
        /*************************************/
        nav_msgs::Odometry odo_msg;
        odo_msg.pose.pose=pos_msg;
        odo_msg.twist.twist=twist_msg.twist;
        /*************************************/
        sensor_msgs::Imu imu_msg;
        imu_msg.angular_velocity=twist_msg.twist.angular;
        imu.data[kraken_sensors::accelX] = imu_msg.linear_acceleration.x=auv._current_accelaration_to_body[0];
        imu.data[kraken_sensors::accelY] = imu_msg.linear_acceleration.y=auv._current_accelaration_to_body[1];
        imu.data[kraken_sensors::accelZ] = imu_msg.linear_acceleration.z=auv._current_accelaration_to_body[2];
        imu_msg.orientation=pos_msg.orientation;
        //imu_pub.publish(imu_msg);
        imu.data[kraken_sensors::roll] = auv._current_position_to_body[3];
        imu.data[kraken_sensors::pitch] = auv._current_position_to_body[4];
        imu.data[kraken_sensors::yaw] = auv._current_position_to_body[5];
//        imu_pub.publish(imu);
        pose_pub.publish(pos_msg);
        if(isLineNeeded)
            odo_pub.publish(odo_msg);
    }
};

void fourThrustCb(const kraken_msgs::forceData4ThrusterConstPtr &msg)
{
    storeforce[0]=msg->data[0];
    storeforce[1]=msg->data[1];
    storeforce[2]=0;
    storeforce[3]=0;
    storeforce[4]=msg->data[2];
    storeforce[5]=msg->data[3];
    //std::cerr<<"callback"<<std::endl;
}

void sixThrustCb(const kraken_msgs::forceData6ThrusterConstPtr &msg)
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
        type=atoi(argv[8]);

    }
    for(int i=1;i<argc&&i<7;i++)
            storeforce[i-1]=atof(argv[i]);
    NodeHandle n;

    Publisher pose_publisher=n.advertise<geometry_msgs::Pose>(topics::SIMULATOR_MODEL_POSE,1);
    Publisher twistS_publisher= n.advertise <geometry_msgs::TwistStamped>(topics::SIMULATOR_MODEL_TWIST,1);
    Publisher odometry_pub= n.advertise <nav_msgs::Odometry>(topics::SIMULATOR_MODEL_DATA_NAVIGATOR,1);
    Publisher imu_pub=n.advertise<kraken_msgs::imuData>(topics::SIMULATOR_MODEL_IMU_DATA,1);
    Subscriber thrust4sub=n.subscribe<kraken_msgs::forceData4Thruster>(topics::SIMULATOR_MODEL_FORCE_DATA_4_THRUSTERS,1,fourThrustCb);
    Subscriber thrust6sub=n.subscribe<kraken_msgs::forceData6Thruster>(topics::SIMULATOR_MODEL_FORCE_DATA_6_THRUSTERS,1,sixThrustCb);

    AuvModelSimple6DoF auv(.05);

    Functor timerCallback(n,auv,pose_publisher,twistS_publisher,odometry_pub,imu_pub,type);
    Timer timer=n.createTimer(ros::Duration(0.1),timerCallback);
    timer.start ();

    spin ();

    return 0;

}
