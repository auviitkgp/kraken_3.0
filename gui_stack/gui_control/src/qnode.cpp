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
    init_argv(argv),_controllerClient("control_client",true),_advancedControllerClient("advancecleint",true)
	{}

QNode::~QNode() {
    if(ros::isStarted()) {
      ros::shutdown(); // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
	wait();
}

bool QNode::init() {
	ros::init(init_argc,init_argv,"App");
	ros::start(); // explicitly needed since our nodehandle is going out of scope.
	ros::NodeHandle n;
	// Add your ros communications here.
	chatter_publisher = n.advertise<std_msgs::String>("chatter", 1000);

	start();
	return true;
}


void QNode::run() {
	ros::Rate loop_rate(1);
	int count = 0;
	while ( ros::ok() ) 
	{
	    
	}
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
    _advancedControllerClient.sendGoal(g1);
}

void QNode::controlGoalCB(float r, float p, float y)
{
    kraken_msgs::controllerGoal g1;
    g1.r=r;
    g1.p=p;
    g1.y=y;
    _controllerClient.sendGoal(g1);
}


}  // namespace App
