#include <ros/ros.h>
#include <iostream>
#include <kraken_msgs/thrusterData6Thruster.h>
#include <sensor_msgs/Joy.h>
#include <resources/topicHeader.h>


#define OFFSET_VALUE 128.0
#define SCALING_FACTOR 102.0
#define MAX_THRUST_FACTOR 0.75


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

kraken_msgs::thrusterData6Thruster _force_sent;

void joyCB(const sensor_msgs::JoyConstPtr &msg)
{


    _force_sent.data[_force_sent.FRONT_Z ]  = OFFSET_VALUE + msg->axes[4] * SCALING_FACTOR * MAX_THRUST_FACTOR;
    _force_sent.data[_force_sent.BACK_Z  ]  = OFFSET_VALUE + msg->axes[4] * SCALING_FACTOR * MAX_THRUST_FACTOR;
    _force_sent.data[_force_sent.LEFT_X  ]  = OFFSET_VALUE - msg->axes[0] * 0.5 * SCALING_FACTOR * MAX_THRUST_FACTOR - msg->axes[3] * 0.5 * SCALING_FACTOR * MAX_THRUST_FACTOR;
    _force_sent.data[_force_sent.RIGHT_X ]  = OFFSET_VALUE + msg->axes[0] * 0.5 * SCALING_FACTOR * MAX_THRUST_FACTOR - msg->axes[3] * 0.5 * SCALING_FACTOR * MAX_THRUST_FACTOR;
    _force_sent.data[_force_sent.FRONT_Y ]  = OFFSET_VALUE + msg->axes[1] * 0.5 * SCALING_FACTOR * MAX_THRUST_FACTOR - msg->axes[0] * 0.5 * SCALING_FACTOR * MAX_THRUST_FACTOR;
    _force_sent.data[_force_sent.BACK_Y  ]  = OFFSET_VALUE + msg->axes[1] * 0.5 * SCALING_FACTOR * MAX_THRUST_FACTOR + msg->axes[0] * 0.5 * SCALING_FACTOR * MAX_THRUST_FACTOR;

}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "manual_control");
    ros::NodeHandle _nh;
    ros::Publisher _force_pub = _nh.advertise<kraken_msgs::thrusterData6Thruster>(topics::CONTROL_PID_THRUSTER6, 100);
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
