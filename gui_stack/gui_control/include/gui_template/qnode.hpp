/**
 * @file /include/App/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef App_QNODE_HPP_
#define App_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <string>
#include <QThread>
#include <QStringListModel>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>


#include <pose_server/KrakenPose.h>
#include <kraken_msgs/advancedControllerAction.h>
#include <kraken_msgs/controllerAction.h>
#include <kraken_msgs/moveAlongLine.h>
#include <kraken_msgs/krakenPose.h>
#include <kraken_msgs/forceData6Thruster.h>
#include <kraken_msgs/moveAlongLine.h>
#include "ui_main_window.h"
/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace gui_template {

/*****************************************************************************
** Class
*****************************************************************************/

typedef actionlib::SimpleActionClient<kraken_msgs::advancedControllerAction> _advancedControllerClientType;
typedef actionlib::SimpleActionClient<kraken_msgs::controllerAction> _controllerClientType;

class QNode : public QThread {
    Q_OBJECT
public:
    QNode(int argc, char** argv ,Ui::MainWindowDesign &uir);
	virtual ~QNode();
	bool init();
	void run();
    void callBackStatePub(const kraken_msgs::krakenPoseConstPtr& msg);
    void callBackForcePub(const kraken_msgs::forceData6ThrusterConstPtr& msg);

Q_SIGNALS:
    void rosShutdown();
    void statePub(const kraken_msgs::krakenPoseConstPtr& msg);
    void forcePub(const kraken_msgs::forceData6ThrusterConstPtr& msg);



public Q_SLOTS:
//    void sendControllGoal(float r,float p,float y);
//    void sendAdvancedControlGoal(float x,float y,float z);
    void advancedGoalCB(float x,float y,float z);
    void controlGoalCB(float r,float p,float y);
    void doneCB(const actionlib::SimpleClientGoalState& state,const kraken_msgs::advancedControllerResultConstPtr& result);
    void feedbackCB(const kraken_msgs::advancedControllerFeedbackConstPtr& msg);
    void activeCB();
    void moveAlongCB(float angle);
    void pauseCB();
private:
	int init_argc;
	char** init_argv;
	ros::Publisher chatter_publisher;
    ros::Subscriber _state_sub,_forceData_sub,_advGoalSub,_advFeedSub,_advStatusSub;
    _advancedControllerClientType* _advancedControllerClient;
    _controllerClientType* _controllerClient;
    ros::ServiceClient _movealongLineSrv;
    Ui::MainWindowDesign &ui;
};

}  // namespace App

#endif /* App_QNODE_HPP_ */
