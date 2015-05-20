#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <actionmsg/binAction.h>

typedef actionlib::SimpleActionClient<actionmsg::binAction> Client;

int main(int argc, char **argv)
{
	ros::init(argc, argv, "bin_client");
	Client _client("bin", true);
	ROS_INFO("bin_client started. Waiting for bin_server.");
	_client.waitForServer();
	ROS_INFO("bin_server started.");
	actionmsg::binGoal _goal;
    _goal.order = 0;
    ROS_INFO("Sending goal - DETECT_BIN.");
    _client.sendGoal(_goal);
    bool _actionStatus = _client.waitForResult(ros::Duration(300.0));
    if(_actionStatus == true)
    {
        actionlib::SimpleClientGoalState _state = _client.getState();
        ROS_INFO("bin_client : Action finished - %s",_state.toString().c_str());
    }
    else
    {
        ROS_INFO("bin_client : Action did not finish within the specified time.");
        _client.cancelGoal();
    }
	return 0;
}
