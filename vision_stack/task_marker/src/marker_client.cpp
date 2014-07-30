#include <ros/ros.h>
#include <ip_msgs/markerAction.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <task_marker/resultheader.h>
#include <task_marker/taskheader.h>

typedef actionlib::SimpleActionClient<ip_msgs::markerAction> Client;

void done_cb(const actionlib::SimpleClientGoalState& state,const ip_msgs::markerResultConstPtr& result)
{
    ROS_INFO("Action server process complete");
}

void feedback_cb(const ip_msgs::markerFeedbackConstPtr& feedback_msg)
{
    ROS_INFO("feed back %f %d",feedback_msg->errorangle,feedback_msg->errorx);
}

void activeCb()
{
  ROS_INFO("Goal just went active");
}

int main(int argc, char ** argv)
{
	ros::init(argc, argv, "markerclient");
    Client _client("marker", true);
    ROS_INFO("marker_client started - waiting for server to start.");
    _client.waitForServer();
    ROS_INFO("markerserver started.");
    ip_msgs::markerGoal _goal;
    _goal.order = DETECT_MARKER;

    _client.sendGoal(_goal,&done_cb,
                     &activeCb ,
                     &feedback_cb);


    bool _actionStatus = _client.waitForResult(ros::Duration(300.0));
    if(_actionStatus == true)
    {
        actionlib::SimpleClientGoalState _state = _client.getState();
        ROS_INFO("marker_client : Action finished: %s",_state.toString().c_str());
    }
    else
    {
        ROS_INFO("marker_client : Action did not finish within specified time.");
        _client.cancelGoal();
    }
    _goal.order = ALIGN_MARKER;
    _client.sendGoal(_goal);
    _actionStatus = _client.waitForResult(ros::Duration(300.0));
    if(_actionStatus == true)
    {
        actionlib::SimpleClientGoalState _state = _client.getState();
        ROS_INFO("marker_client : Action finished: %s",_state.toString().c_str());
    }
    else
    {
        ROS_INFO("marker_client : Action did not finish within specified time.");
        _client.cancelGoal();
    }

	return 0;
}
