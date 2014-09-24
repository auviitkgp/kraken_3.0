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
#include <resources/topicHeader.h>
#include "../include/gui_template/qnode.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace gui_template {

/*****************************************************************************
** Implementation
*****************************************************************************/

QNode::QNode(int argc, char** argv ) :
	init_argc(argc),
    init_argv(argv)
	{}

QNode::~QNode() {
    if(ros::isStarted()) {
      ros::shutdown(); // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
	wait();
}

bool QNode::init() {
    ros::init(init_argc,init_argv,"Control_gui");
	ros::start(); // explicitly needed since our nodehandle is going out of scope.
	ros::NodeHandle n;
	// Add your ros communications here.

  //_ros_subscriber_thruster_6_data = n.subscribe<kraken_msgs::thrusterData6Thruster>(_ros_topic_thruster_6_data.c_str(),1,&QNode::callBackThruster6Data,this);
    _controllerClient=new _controllerClientType("controller_client",true);
    _advancedControllerClient=new _advancedControllerClientType("advanced_controller_client",true);
    _state_sub=n.subscribe<kraken_msgs::krakenPose>(topics::NAV_POSE_ESTIMATED,5,&QNode::callBackStatePub,this);
    _forceData_sub=n.subscribe<kraken_msgs::forceData6Thruster>(topics::SIMULATOR_MODEL_FORCE_DATA_6_THRUSTERS,5,&QNode::callBackForcePub,this);
	start();
	return true;
}


void QNode::run() {
    ros::spin();
    std::cout << "Ros shutdown, proceeding to close the gui." << std::endl;
    Q_EMIT rosShutdown(); // used to signal the gui for a shutdown (useful to roslaunch)
}

void QNode::callBackStatePub(const kraken_msgs::krakenPoseConstPtr &msg)
{
    Q_EMIT statePub(msg);
}

void QNode::callBackForcePub(const kraken_msgs::forceData6ThrusterConstPtr &msg)
{
    Q_EMIT forcePub(msg);
}

void QNode::advancedGoalCB(float x, float y, float z)
{
    kraken_msgs::advancedControllerGoal g1;
    g1.x=x;
    g1.y=y;
    g1.depth=z;
    g1.flag=0;
    _advancedControllerClient->sendGoal(g1);
}

void QNode::controlGoalCB(float r, float p, float y)
{
    kraken_msgs::controllerGoal g1;
    g1.r=r;
    g1.p=p;
    g1.y=y;
    _controllerClient->sendGoal(g1);
}


}  // namespace App
