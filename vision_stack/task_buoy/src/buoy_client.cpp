#include <ros/ros.h>
#include <actionmsg/buoyAction.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <task_buoy/resultheader.h>
#include <task_buoy/taskheader.h>

typedef actionlib::SimpleActionClient<actionmsg::buoyAction> Client;

int main(int argc, char ** argv)
{
    ros::init(argc, argv, "buoy_client");
    Client _client("buoy", true);
    ROS_INFO("buoy_client started. Waiting for buoy_server.");
    _client.waitForServer();
    ROS_INFO("buoy_server started.");
    actionmsg::buoyGoal _goal;
    _goal.order = DETECT_BUOY;
    ROS_INFO("Sending goal - DETECT_BUOY.");
    _client.sendGoal(_goal);
    bool _actionStatus = _client.waitForResult(ros::Duration(3000.0));
    if(_actionStatus == true)
    {
        actionlib::SimpleClientGoalState _state = _client.getState();
        ROS_INFO("buoy_client : Action finished - %s",_state.toString().c_str());
    }
    else
    {
        ROS_INFO("buoy_client : Action did not finish within the specified time.");
        _client.cancelGoal();
    }
   ROS_INFO("Sending goal - ALIGN_BUOY"); 
    _goal.order = ALIGN_BUOY;
    _client.sendGoal(_goal);
    _actionStatus = _client.waitForResult(ros::Duration(3000.0));
    if(_actionStatus == true)
    {
        actionlib::SimpleClientGoalState _state = _client.getState();
        ROS_INFO("buoy_client : Action finished - %s",_state.toString().c_str());
    }
    else
    {
        ROS_INFO("buoy_client : Action did not finish within the specified time.");
        _client.cancelGoal();
    }

    return 0;
}
