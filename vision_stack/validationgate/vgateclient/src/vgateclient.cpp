#include <iostream>
#include <ros/ros.h>
#include <actionmsg/vgateAction.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <vgateclient/resultheader.h>
#include <vgateclient/taskheader.h>

using namespace std;

typedef actionlib::SimpleActionClient<actionmsg::vgateAction> Client;

int main(int argc, char ** argv)
{
	ros::init(argc, argv, "vgateclient");
    Client _client("vgate");
    ROS_INFO("vgatenclient started : waiting for server to start.");
    _client.waitForServer();
    ROS_INFO("vgateserver started.");
    actionmsg::vgateGoal _goal;
    _goal.order = DETECT_VGATE;
    _client.sendGoal(_goal);
    bool _actionStatus = _client.waitForResult(ros::Duration(15.0));
    if(_actionStatus)
    {
        actionlib::SimpleClientGoalState _state = _client.getState();
        ROS_INFO("vgateclient : Action finished: %s",_state.toString().c_str());
    }
    else
    {
        ROS_INFO("vgateclient : Action did not finish within specified time.");
        _client.cancelGoal();
    }

    _goal.order = ALLIGN_VGATE;
    _client.sendGoal(_goal);
    _actionStatus = _client.waitForResult(ros::Duration(15.0));
    if(_actionStatus == true)
    {
        actionlib::SimpleClientGoalState _state = _client.getState();
        ROS_INFO("vgateclient : Action finished: %s",_state.toString().c_str());
    }
    else
    {
        ROS_INFO("vgateclient : Action did not finish within specified time.");
        _client.cancelGoal();
    }
	return 0;
}
