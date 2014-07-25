#include <ros/ros.h>
#include <iostream>
#include <kraken_msgs/forceData6Thruster.h>
#include <sensor_msgs/Joy.h>

#include <resources/topicHeader.h>

using namespace std;
kraken_msgs::forceData6Thruster _force_sent;
float _received[11];

void joyCB(const sensor_msgs::JoyConstPtr &msg)
{

    _received[0] = msg->axes[0];
    _received[1] = msg->axes[1];
    _received[2] = msg->axes[2];
    _received[3] = msg->axes[3];
    _received[4] = msg->axes[4];

    _received[5] = msg->buttons[0];
    _received[6] = msg->buttons[1];
    _received[7] = msg->buttons[2];
    _received[8] = msg->buttons[3];
    _received[9] = msg->buttons[4];
    _received[10] = msg->buttons[5];
}

int main(int argc, char *argv[]){
    ros::init(argc, argv, "joystick_control");
	ros::NodeHandle _nh;
    ros::Publisher _force_pub = _nh.advertise<kraken_msgs::forceData6Thruster>(topics::SIMULATOR_MODEL_FORCE_DATA_6_THRUSTERS, 100);
    ros::Subscriber _joy_sub = _nh.subscribe<sensor_msgs::Joy>("/joy", 100, joyCB);
    ros::Rate _looprate(10);

    while(ros::ok())
    {
        _force_sent.data[0] = _received[1] * 32.0;
        _force_sent.data[1] = _received[1] * 32.0;
        _force_sent.data[2] = - _received[3] * 16.0 - _received[0];
        _force_sent.data[3] = - _received[3] * 16.0 + _received[0];
        _force_sent.data[4] = _received[4] * 12;
        _force_sent.data[5] = _received[4] * 12;
        _force_pub.publish(_force_sent);
         ros::spinOnce();
        _looprate.sleep();
    }
	return 0;
}
