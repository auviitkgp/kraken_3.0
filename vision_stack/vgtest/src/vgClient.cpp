#ifndef CLIENT_VG
#define CLIENT_VG

#include <vgtest/vgheader.h>
#include <ip_msgs/vgateAction.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <ros/ros.h>
#include <vgtest/vgGoal.h>
#include <vgtest/vgResult.h>


typedef actionlib::SimpleActionClient<ip_msgs::vgateAction> Client;

int main(int argc, char ** argv)
{
    ros::init(argc, argv, "vgclient");
    Client _client("gate_server", true);
    ROS_INFO("The Client has started and waiting for server to start ....");
    _client.waitForServer();
   ip_msgs::vgateGoal _goal;
    cout << "action client";
   if(argc >= 2)
   {
     _goal.gate = argc;
   }
   else
       _goal.gate = 1;

    _client.sendGoal(_goal);
    bool _actionStatus = _client.waitForResult(ros::Duration(30.0));
  //  ROS_INFO("error x and error y : %f  and %f", )
    if(_actionStatus == true)
    {
        actionlib::SimpleClientGoalState _state = _client.getState();
        ROS_INFO("Action finished: %s",_state.toString().c_str());
    }
    else
    {
        ROS_INFO("The action did not finish within the specified time");
        _client.cancelGoal();
    }

    return 0;
}

#endif CLIENT_VG
