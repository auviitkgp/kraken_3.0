#include <ros/ros.h>
#include <actionmsg/buoyAction.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <task_buoy/resultheader.h>
#include <task_buoy/taskheader.h>

#include <resources/topicHeader.h>
#include <kraken_msgs/absoluteRPY.h>
#include <kraken_msgs/setYawAction.h>

typedef actionlib::SimpleActionClient<actionmsg::buoyAction> Client;
typedef actionlib::SimpleActionClient<kraken_msgs::setYawAction> Client1;

double current_yaw;

void getyawCallback(const kraken_msgs::absoluteRPY& msg)
{
    current_yaw = msg.yaw;
}

int main(int argc, char ** argv)
{
    ros::init(argc, argv, "buoy_node");

    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe(topics::ABSOLUTE_RPY, 1000, getyawCallback);

    // Start Service

    Client detect_client("buoy", true);
    ROS_INFO("buoy_client has started ... Waiting for the server to start");
    detect_client.waitForServer();
    ROS_INFO("buoy_server has started.");
    actionmsg::buoyGoal _goal;
    _goal.order = DETECT_BUOY;
    ROS_INFO("Sending goal - DETECT_BUOY.");
    detect_client.sendGoal(_goal);
    bool _actionStatus = detect_client.waitForResult(ros::Duration(300.0));

    if(_actionStatus == true)
    {
        // Stop Service

        actionlib::SimpleClientGoalState _state = detect_client.getState();
        ROS_INFO("buoy_client : Action finished - %s",_state.toString().c_str());

        // Get the yaw here and store it
        // Global variable is being set to yaw whenever callback function is called

        Client1 yaw_client("setYaw", true);
        ROS_INFO("yaw_client has started ... Waiting for the server to start");
        yaw_client.waitForServer();
        ROS_INFO("yaw_server has started.");
        kraken_msgs::setYawGoal _goal1;
        _goal1.yaw = current_yaw;
        ROS_INFO("Secding goal to set yaw at %f", current_yaw);
        yaw_client.sendGoal(_goal1);
        _actionStatus = yaw_client.waitForResult();

    }
    else
    {
        ROS_INFO("buoy_client : No buoy found in the area");
        detect_client.cancelGoal();
    }

    return 0;
}