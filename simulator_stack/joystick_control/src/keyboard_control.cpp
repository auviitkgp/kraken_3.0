#include <ros/ros.h>
#include <iostream>
#include <kraken_msgs/forceData6Thruster.h>
#include <std_msgs/String.h>

#include <resources/topicHeader.h>

#include <string.h>

/**********

	Force Defination
	force[0] = forward thruster on left side, positive value takes vehicle forwards
	force[1] = forward thruster on right side, positive value takes vehicle forwards
	force[2] = sway thruster on front side, positive value takes vehicle rightwards
	force[3] = sway thruster on back side, positive value takes vehicle rightwards
	force[4] = depth thruster on back side, positive value takes vehicle downwards
	force[5] = depth thruster on front side, positive value takes vehicle downwards

	W - ++ 0 ; ++ 1
	S - -- 0 ; -- 1
	A - -- 2 ;
	D - ++ 2 ;
	T - ++ 4 ; ++ 5
	G - -- 4 ; -- 5

 *****************/


using namespace std;
kraken_msgs::forceData6Thruster _force_sent;
float _received[6]= {0.0}, _forcePresentData[6]= {0.0};
const int BASE = 2;
const float SIDE_BASE=0.5;
void storePresentForceData(const kraken_msgs::forceData6Thruster force)
{
    /*
    	 std::cout << force.data[0] << "\n";
    	 std::cout << force.data[1] << "\n";
    	 std::cout << force.data[2] << "\n";
    	 std::cout << force.data[3] << "\n";
    	 std::cout << force.data[4] << "\n";
    	 std::cout << force.data[5] << "\n";
    	 */
    _forcePresentData[0] = force.data[0];
    _forcePresentData[1] = force.data[1];
    _forcePresentData[2] = force.data[2];
    _forcePresentData[3] = force.data[3];
    _forcePresentData[4] = force.data[4];
    _forcePresentData[5] = force.data[5];
}

void keyCB(const std_msgs::String::ConstPtr& msg)
{

    // std::msgs::String a = msg->data.c_str();

    ROS_INFO("I heard: [%s]", msg->data.c_str());
    // ROS_INFO("I heard: [%s]", a);

    if(strcmp(msg->data.c_str(), "left") == 0)
    {
        std::cout << "left";
        _received[2] = _forcePresentData[2] - SIDE_BASE;
    }

    if(strcmp(msg->data.c_str(), "right") == 0)
    {
        std::cout << "right";
        _received[2] = _forcePresentData[2] + SIDE_BASE;
    }

    if(strcmp(msg->data.c_str(), "forward") == 0)
    {
        std::cout << "forward";
        _received[0] = _forcePresentData[0] + BASE;
        _received[1] = _forcePresentData[1] + BASE;
    }

    if(strcmp(msg->data.c_str(), "backward") == 0)
    {
        std::cout << "backward";
        _received[0] = _forcePresentData[0] - BASE;
        _received[1] = _forcePresentData[1] - BASE;
    }

    if(strcmp(msg->data.c_str(), "top") == 0)
    {
        std::cout << "top";
        _received[4] = _forcePresentData[4] - BASE;
        _received[5] = _forcePresentData[5] - BASE;
    }

    if(strcmp(msg->data.c_str(), "bottom") == 0)
    {
        std::cout << "bottom";
        _received[4] = _forcePresentData[4] + BASE;
        _received[5] = _forcePresentData[5] + BASE;
    }

    if(strcmp(msg->data.c_str(), "STOP") == 0)
    {
        std::cout << "STOP";
        _received[0] = 0;
        _received[1] = 0;
        _received[2] = 0;
        _received[3] = 0;
        _received[4] = 0;
        _received[5] = 0;
    }

}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "keyboard_control");
    ros::NodeHandle _nh;
    ros::Publisher _force_pub = _nh.advertise<kraken_msgs::forceData6Thruster>(topics::SIMULATOR_MODEL_FORCE_DATA_6_THRUSTERS, 100);
    ros::Subscriber sub1 = _nh.subscribe(topics::SIMULATOR_MODEL_FORCE_DATA_6_THRUSTERS, 1000, storePresentForceData);

    // ros::Subscriber _joy_sub = _nh.subscribe<sensor_msgs::Joy>("/keyboard", 100, joyCB);
    ros::Subscriber sub = _nh.subscribe("keyboard", 1000, keyCB);

    ros::Rate _looprate(10);

    while(ros::ok())
    {
        _force_sent.data[0] = _received[0];
        _force_sent.data[1] = _received[1];
        _force_sent.data[2] = _received[2];
        _force_sent.data[3] = _received[3];
        _force_sent.data[4] = _received[4];
        _force_sent.data[5] = _received[5];

        _force_pub.publish(_force_sent);
        ros::spinOnce();
        _looprate.sleep();
    }

    return 0;
}
