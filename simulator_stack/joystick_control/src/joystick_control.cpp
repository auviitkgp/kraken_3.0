#include <ros/ros.h>
#include <iostream>
#include <kraken_msgs/forceData6Thruster.h>
#include <sensor_msgs/Joy.h>
#include <resources/topicHeader.h>

/*
Xbox Controller Mapping
----------------------------
Axis 0: Left Stick Horizontal
Axis 1: Left Stick Vertical
Axis 2: Left Trigger
Axis 3: Right Stick Horizontal
Axis 4: Right Stick Vertical
Axis 5: Right Trigger
Axis 6: D-pad Horizontal
Axis 7: D-pad Vertical

Button 0: A
Button 1: B
Button 2: X
Button 3: Y
Button 4: Left Bumper
Button 5: Right Bumper
Button 6: Back
Button 7: Start
Button 8: Xbox Guide
*/
kraken_msgs::forceData6Thruster _force_sent;

void joyCB(const sensor_msgs::JoyConstPtr &msg)
{

    _force_sent.data[0] = 	msg->axes[1] * 32.0;
    _force_sent.data[1] = 	msg->axes[1] * 32.0;
    _force_sent.data[2] = - msg->axes[3] * 16.0 - msg->axes[0];
    _force_sent.data[3] = - msg->axes[3] * 16.0 + msg->axes[0];
    _force_sent.data[4] = 	msg->axes[4] * 12;
    _force_sent.data[5] = 	msg->axes[4] * 12;
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "manual_control");
    ros::NodeHandle _nh;
    ros::Publisher _force_pub = _nh.advertise<kraken_msgs::forceData6Thruster>(topics::SIMULATOR_MODEL_FORCE_DATA_6_THRUSTERS, 100);
    ros::Subscriber _joy_sub = _nh.subscribe<sensor_msgs::Joy>("/joy", 100, joyCB);
    ros::Rate _looprate(10);

    while(ros::ok())
    {
        _force_pub.publish(_force_sent);
        ros::spinOnce();
        _looprate.sleep();
    }

    return 0;
}
