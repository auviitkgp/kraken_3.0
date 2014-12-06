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

QNode::QNode(int argc, char** argv, Ui::MainWindowDesign &uir ) :
	init_argc(argc),
    init_argv(argv),
    ui(uir)
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
    _controllerClient=new _controllerClientType(topics::CONTROL_SETPOINT_ACTION,true);
    _advancedControllerClient=new _advancedControllerClientType(topics::CONTROL_ADVANCEDCONTROLLER_ACTION,true);
    _state_sub=n.subscribe<kraken_msgs::krakenPose>(topics::NAV_POSE_ESTIMATED,5,&QNode::callBackStatePub,this);
    _forceData_sub=n.subscribe<kraken_msgs::forceData6Thruster>(topics::SIMULATOR_MODEL_FORCE_DATA_6_THRUSTERS,5,&QNode::callBackForcePub,this);
    _movealongLineSrv=n.serviceClient<kraken_msgs::moveAlongLine>(topics::CONTROL_MOVEALONG_SERV);
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
//    _advancedControllerClient->sendGoal(g1,&QNode::doneCB,_advancedControllerClientType::SimpleActiveCallback,&QNode::feedbackCB);
    ROS_INFO("sending goal %f %f %f\n",x,y,z);
    _advancedControllerClient->sendGoal(g1);/*,
                                        /*&QNode::doneCB,
                                        &QNode::activeCB,
                                        &QNode::feedbackCB)*/;
}

void QNode::controlGoalCB(float r, float p, float y)
{
    kraken_msgs::controllerGoal g1;
    g1.r=r;
    g1.p=p;
    g1.y=y;
    g1.d=0;
    _controllerClient->sendGoal(g1);
}

void QNode::doneCB(const actionlib::SimpleClientGoalState &state, const kraken_msgs::advancedControllerResultConstPtr &result)
{
    ui.advancedfeedback->append(QString("\nState=")+QString::fromStdString(state.getText())+QString("\n Result="));
    ui.advancedfeedback->append(QString::number(result->time_taken));
}

void QNode::feedbackCB(const kraken_msgs::advancedControllerFeedbackConstPtr &msg)
{
    ui.advancedfeedback->append(QString::number(msg->running_time));
    ui.advancedfeedback->append("\n");
}

void QNode::activeCB()
{

}

void QNode::moveAlongCB(float angle)
{
    kraken_msgs::moveAlongLine srv;
    srv.request.angle=angle;
    srv.request.type=1;//0 to pause and 1 to move
    if(_movealongLineSrv.call(srv))
    {
        QString str=QString::number(angle);
        ui.textBrowser->append("moving along line with angle clockwise "+str+"\n");
    }
    else
        ui.textBrowser->append("error in calling move along service!!!!");

}

void QNode::pauseCB()
{
    kraken_msgs::moveAlongLine srv;
    srv.request.type=0;
    if(_movealongLineSrv.call(srv))
    {
        ui.textBrowser->append("vehicle stopped moving");
    }
    else
        ui.textBrowser->append("error in stopping vehicle!!!!");
}



}  // namespace App
