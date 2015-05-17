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
	
}
