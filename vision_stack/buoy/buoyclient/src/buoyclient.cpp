#include <ros/ros.h>
#include <actionmsg/buoyAction.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <buoyclient/resultheader.h>
#include <buoyclient/taskheader.h>

typedef actionlib::SimpleActionClient<actionmsg::buoyAction> Client;

int main(int argc, char ** argv)
{
    ros::init(argc, argv, "buoyclient");
    Client _client("buoy", true);
    ROS_INFO("buoyclient started. Waiting for BuoyServer.");
    _client.waitForServer();
    ROS_INFO("buoyserver started.");
    actionmsg::buoyGoal _goal;
    _goal.order = DETECT_BUOY;
    _client.sendGoal(_goal);
    bool _actionStatus = _client.waitForResult(ros::Duration(15.0));
    if(_actionStatus == true)
    {
        actionlib::SimpleClientGoalState _state = _client.getState();
        ROS_INFO("buoyclient : Action finished - %s",_state.toString().c_str());
    }
    else
    {
        ROS_INFO("buoyclient : Action did not finish within the specified time.");
        _client.cancelGoal();
    }
    _goal.order = ALLIGN_BUOY;
    _client.sendGoal(_goal);
    _actionStatus = _client.waitForResult(ros::Duration(15.0));
    if(_actionStatus == true)
    {
        actionlib::SimpleClientGoalState _state = _client.getState();
        ROS_INFO("buoyclient : Action finished - %s",_state.toString().c_str());
    }
    else
    {
        ROS_INFO("buoyclient : Action did not finish within the specified time.");
        _client.cancelGoal();
    }

    return 0;
}
