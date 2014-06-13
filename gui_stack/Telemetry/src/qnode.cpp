/**
 * @file /src/qnode.cpp
 *
 * @brief Ros communication central!
 *
 * @date February 2011
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <ros/network.h>
#include <string>
#include <std_msgs/String.h>
#include <sstream>
#include "../include/Telemetry/qnode.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace Telemetry {

/*****************************************************************************
** Implementation
*****************************************************************************/

QNode::QNode(int argc, char** argv ) :
	init_argc(argc),
	init_argv(argv)
	{}

QNode::~QNode() 
{
    if(ros::isStarted()) 
    {
      ros::shutdown(); // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
    wait();
}

bool QNode::init() 
{
    ros::init(init_argc,init_argv,"Telemetry");
    ros::start(); // explicitly needed since our nodehandle is going out of scope.
    ros::NodeHandle n;
    _ros_subscriber_front_image = n.subscribe<sensor_msgs::Image>(_ros_topic_front_image.c_str(), 1,&QNode::callBackFrontImage,this);
    _ros_subscriber_bottom_image = n.subscribe<sensor_msgs::Image>(_ros_topic_bottom_image.c_str(), 1,&QNode::callBackBottomImage,this);
    _ros_subscriber_thruster_4_data = n.subscribe<kraken_msgs::thrusterData4Thruster>(_ros_topic_thruster_4_data.c_str(),1,&QNode::callBackThruster4Data,this);
    _ros_subscriber_thruster_6_data = n.subscribe<kraken_msgs::thrusterData6Thruster>(_ros_topic_thruster_6_data.c_str(),1,&QNode::callBackThruster6Data,this);
    _ros_subscriber_kraken_current_pose = n.subscribe<kraken_msgs::krakenPose>(_ros_topic_kraken_current_pose.c_str(),1,&QNode::callBackKrakenCurrentPose,this);
    _ros_subscriber_set_pose = n.subscribe<kraken_msgs::krakenPose>(_ros_topic_kraken_set_pose.c_str(),1,&QNode::callBackKrakenSetPose,this);
    // Add your ros communications here.
    ROS_INFO("Telemetry Started");
    start();
    return true;
}


void QNode::run() 
{
    ros::spin();
    std::cout << "Ros shutdown, proceeding to close the gui." << std::endl;
    Q_EMIT rosShutdown(); // used to signal the gui for a shutdown (useful to roslaunch)
}

void QNode::callBackBottomImage(const sensor_msgs::ImageConstPtr &msg)
{
  Q_EMIT updateBottomImage(msg);
}

void QNode::callBackFrontImage(const sensor_msgs::ImageConstPtr &msg)
{
  Q_EMIT updateFrontImage(msg);;
}

void QNode::callBackKrakenCurrentPose(const kraken_msgs::krakenPoseConstPtr &msg)
{
  Q_EMIT updateCurrentPose(msg);
}

void QNode::callBackKrakenSetPose(const kraken_msgs::krakenPoseConstPtr &msg)
{
  Q_EMIT updateSetPose(msg);
}

void QNode::callBackThruster4Data(const kraken_msgs::thrusterData4ThrusterConstPtr &msg)
{
  
}

void QNode::callBackThruster6Data(const kraken_msgs::thrusterData6ThrusterConstPtr &msg)
{
  
}

}  // namespace App
