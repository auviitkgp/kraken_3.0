#include <ros/ros.h>
#include <actionmsg/buoyAction.h>
#include <kraken_msgs/scanAction.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <task_buoy/resultheader.h>
#include <task_buoy/taskheader.h>

#include <resources/topicHeader.h>
#include <kraken_msgs/absoluteRPY.h>
#include <kraken_msgs/setYawAction.h>

#include <signal.h>

typedef actionlib::SimpleActionClient<actionmsg::buoyAction> Client;
typedef actionlib::SimpleActionClient<kraken_msgs::setYawAction> Client1;
typedef actionlib::SimpleActionClient<kraken_msgs::scanAction> Client2;

bool dummy()
{
    char** dum_arg;
    int x;
    ros::init(x, dum_arg, "buoy_node");
    return true;
}

bool res = dummy();

Client detect_client("buoy", true);
Client1 yaw_client("setYaw", true);
Client2 scan_start("scanning_server", true);

void my_handler(int s)
{
    printf("Caught Signal %d\n", s);
    detect_client.cancelGoal();
    yaw_client.cancelGoal();
    scan_start.cancelGoal();
    exit(1);
}

double current_yaw;

void getyawCallback(const kraken_msgs::absoluteRPY& msg)
{
    current_yaw = msg.yaw;
}

int main(int argc, char ** argv)
{
    signal (SIGINT,my_handler);

    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe(topics::ABSOLUTE_RPY, 1000, getyawCallback);

    ROS_INFO("Scanning client has started ... Waiting for the server to start");
    scan_start.waitForServer();
    ROS_INFO("Scanning server has started");
    kraken_msgs::scanGoal _goal2;
    _goal2.direction = -1;
    _goal2.speed = -1;
    ROS_INFO("Sending angular speed for the bot to rotate at.");
    scan_start.sendGoal(_goal2);

    ROS_INFO("buoy_client has started ... Waiting for the server to start");
    detect_client.waitForServer();
    ROS_INFO("buoy_server has started.");
    actionmsg::buoyGoal _goal;
    _goal.order = DETECT_BUOY;
    ROS_INFO("Sending goal - DETECT_BUOY.");
    detect_client.sendGoal(_goal);

    // The duration given here should be the time taken by AUV to complete one full rotation.
    // We need to consider the time taken by IP to scan the frame and decide on the rotational speed accordingly.

    bool _actionStatus = detect_client.waitForResult(ros::Duration(300.0)); 

    if(_actionStatus == true)
    {
    	scan_start.cancelGoal();

        actionlib::SimpleClientGoalState _state = detect_client.getState();
        ROS_INFO("buoy_client : Action finished - %s",_state.toString().c_str());

        // Get the yaw here and store it
        // Global variable is being set to yaw whenever callback function is called

        ROS_INFO("yaw_client has started ... Waiting for the server to start");
        yaw_client.waitForServer();
        ROS_INFO("yaw_server has started.");
        kraken_msgs::setYawGoal _goal1;
        _goal1.yaw = current_yaw;
        ROS_INFO("Secding goal to set yaw at %f", current_yaw);
        yaw_client.sendGoal(_goal1);
        //_actionStatus = yaw_client.waitForResult();
        return 0;

    }
    else
    {
        ROS_INFO("buoy_client : No buoy found in the area");
        detect_client.cancelGoal();
        scan_start.cancelGoal();
    }

    return 0;
}
