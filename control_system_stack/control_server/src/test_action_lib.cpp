#include <control_server/ControlServer.h>
#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <kraken_msgs/advancedControllerAction.h>
#include <kraken_msgs/advancedControllerGoal.h>

int main(int argc,char** argv)
{
    ros::init(argc, argv, "test_fibonacci");

    // create the action client
    // true causes the client to spin its own thread
    actionlib::SimpleActionClient<kraken_msgs::advancedControllerAction> ac("/kraken/advance_controller_action", true);

    ROS_INFO("Waiting for action server to start.");
    // wait for the action server to start
    ac.waitForServer(); //will wait for infinite time

    ROS_INFO("Action server started, sending goal.");
    // send a goal to the action
    kraken_msgs::advancedControllerGoal goal;
    goal.depth = atoi(argv[1]);
    goal.x = 1.0;
    goal.y = 2.0;
    ac.sendGoal(goal);
    //ac.sendGoal(goal);
    ROS_INFO("Goal sent");
    //wait for the action to return
    bool finished_before_timeout = ac.waitForResult(ros::Duration(30.0));

    if (finished_before_timeout)
    {
        actionlib::SimpleClientGoalState state = ac.getState();
        ROS_INFO("Action finished: %s",state.toString().c_str());
    }
    else
    {
        ac.cancelGoal();
        ROS_INFO("Action did not finish before the time out.");
    }
    return 0;
}
