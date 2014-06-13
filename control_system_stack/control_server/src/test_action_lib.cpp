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
    actionlib::SimpleActionClient<kraken_msgs::controllerAction> ac1("/kraken/controller_action", true);
    ROS_INFO("Waiting for action server to start.");
    // wait for the action server to start
    ac.waitForServer(); //will wait for infinite time
    ac1.waitForServer(); //will wait for infinite time

    ROS_INFO("Action server started, sending goal.");
    // send a goal to the action
    kraken_msgs::advancedControllerGoal goal;
    kraken_msgs::controllerGoal goal1;
    if(atoi(argv[1])==1)
    {
        goal.flag = atof(argv[2]);
        goal.x = atof(argv[3]);
        goal.y = atof(argv[4]);
        goal.depth = atof(argv[5]);

        ac.sendGoal(goal);
        //ac.sendGoal(goal);
        ROS_INFO("Goal sent");
        std::cerr <<"x:"<<goal.x<<"\ty:"<<goal.y<<"\tz:"<<goal.depth<<std::endl;
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
    }
    else
    {
        goal1.p = atof(argv[3]);
        goal1.r = atof(argv[4]);
        goal1.y = atof(argv[5]);
        goal1.d = 0.0;
        ac1.sendGoal(goal1);
        //ac.sendGoal(goal);
        ROS_INFO("Goal sent");
        std::cerr <<"r:"<<goal1.p<<"\tp:"<<goal1.r<<"\ty:"<<goal1.y<<std::endl;
        //wait for the action to return
        bool finished_before_timeout = ac1.waitForResult(ros::Duration(20.0));

        if (finished_before_timeout)
        {
            actionlib::SimpleClientGoalState state = ac1.getState();
            ROS_INFO("Action finished: %s",state.toString().c_str());
        }
        else
        {
            ac1.cancelGoal();
            ROS_INFO("Action did not finish before the time out.");
        }
    }
    return 0;
}
