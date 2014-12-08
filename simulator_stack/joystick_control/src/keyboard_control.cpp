#include <ros/ros.h>
#include <iostream>
#include <kraken_msgs/forceData6Thruster.h>
#include <std_msgs/String.h>

#include <resources/topicHeader.h>

#include <string.h>

using namespace std;
kraken_msgs::forceData6Thruster _force_sent;
float _received[6]={0.0};
const int BASE = 10;

// _received[0] = 0.0;
// _received[1] = 0.0;
// _received[2] = 0.0;
// _received[3] = 0.0;
// _received[4] = 0.0;
// _received[5] = 0.0;

void keyCB(const std_msgs::String::ConstPtr& msg)
{

    // std::msgs::String a = msg->data.c_str();

    ROS_INFO("I heard: [%s]", msg->data.c_str());
    // ROS_INFO("I heard: [%s]", a);

    if(strcmp(msg->data.c_str(), "left") == 0){
        std::cout << "left";
        _received[2] = -1 * BASE;
        _received[3] = BASE;
    }

    if(strcmp(msg->data.c_str(), "right") == 0){
        std::cout << "right";
        _received[2] = BASE;
        _received[3] = -1 * BASE;
    }

    if(strcmp(msg->data.c_str(), "forward") == 0){
        std::cout << "forward";
        _received[0] = BASE;
        _received[1] = BASE;
    }

    if(strcmp(msg->data.c_str(), "backward") == 0){
        std::cout << "backward";
        _received[0] = -1 * BASE;
        _received[1] = -1 * BASE;
    }

}

int main(int argc, char *argv[]){
    ros::init(argc, argv, "keyboard_control");
    ros::NodeHandle _nh;
    ros::Publisher _force_pub = _nh.advertise<kraken_msgs::forceData6Thruster>(topics::SIMULATOR_MODEL_FORCE_DATA_6_THRUSTERS, 100);
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

    // ros::spin();

    return 0;
}
