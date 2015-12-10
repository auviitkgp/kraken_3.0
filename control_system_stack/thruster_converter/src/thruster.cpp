/*
  This package takes the data (thrusterData4Truster or thrusterData6Thruster : othh float32 array) and converts
  it into data for real-word thrusters(seabotix) which is a hexaecimal input.
  Zero Velocity for thruster is defined as offset
  Max is the max possible value for forward velocity
  Min is the min possible value for backward velocity
  Converter converts the float input into uint8 data for further manipulation

  Thruster output is initialised as offset so as to keep consitency for both types
  of input data(4 thrusters and 6 thrusters)
*/
#include <string>
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <ros/ros.h>
#include <kraken_msgs/seabotix.h>
#include <kraken_msgs/thrusterData4Thruster.h>
#include <kraken_msgs/thrusterData6Thruster.h>
#include <SerialPort/SerialPort.h>
#include <resources/topicHeader.h>
#include <resources/tools.h>

float converter = 1.0;
uint8_t offset = 0x80;
uint8_t max = 0xE6;   //Maximum forward thrust
uint8_t min = 0x19;   //Maximum backward thrust
kraken_msgs::seabotix _output;

void thruster4callback(const kraken_msgs::thrusterData4ThrusterConstPtr &msg)
{
    float inData[4] = {0,0,0,0};
    uint8_t store;


    for(int j=0; j<6 ; j++)
    {
        _output.data[j] = offset;
    }

    for(int i = 0; i<4 ; i++ )
    {
        inData[i] = msg->data[i];
        store = uint8_t((converter*inData[i]>0xE6-0x80?(0xE6-0x80):converter*inData[i])+offset);
        store = uint8_t((converter*inData[i]<0x19-0x80?(0x19-0x80):converter*inData[i])+offset);

        if (store > max)
        {
            store = max;
        }

        if (store < min)
        {
            store = min;
        }

        _output.data[i] = store;
    }


}

void thruster6callback(const kraken_msgs::thrusterData6ThrusterConstPtr msg)
{
    float inData[6] = {0.0,0.0,0.0,0.0,0.0,0.0};
    uint8_t store;


    for(int j=0; j<6 ; j++)
    {
        _output.data[j] = offset;
    }

    for(int i = 0; i<6 ; i++ )
    {
        inData[i] = msg->data[i];
        ROS_DEBUG("indata[%d] : %f",i,inData[i]);
        store = uint8_t((converter*inData[i]>(0xE6-0x80)?(0xE6):converter*inData[i]+0x80));
        store = uint8_t((converter*inData[i]<(0x19-0x80)?(0x19):converter*inData[i]+0x80));
        ROS_DEBUG("store : %d",store);

        if (store > max)
        {
            store = max;
        }

        if (store < min)
        {
            store = min;
        }

        _output.data[i] = store;
    }

}

int main(int argc,char** argv)
{

    ros::init(argc ,argv, "seabotixConverter");

    if(tools::getVerboseTag(argc, argv) && ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug))
    {
        ros::console::notifyLoggerLevelsChanged();
    }

    ROS_DEBUG("The verbosity of this node is set to DEBUG");

    ros::NodeHandle n;
    ros::Subscriber _sub4 = n.subscribe<kraken_msgs::thrusterData4Thruster>(topics::CONTROL_PID_THRUSTER4,2,thruster4callback);
    ros::Subscriber _sub6 = n.subscribe<kraken_msgs::thrusterData6Thruster>(topics::CONTROL_PID_THRUSTER6,2,thruster6callback);
    ros::Publisher _pub = n.advertise<kraken_msgs::seabotix>(topics::CONTROL_SEABOTIX,2);

//    Serial arduino;

    char* ptr_rat = getenv("ROS_RATE");
    double temp_rate;
    if(ptr_rat==NULL)
        temp_rate = 8.0;
    else
    {
        std::string str_rat(ptr_rat);
        temp_rate = atof(str_rat.c_str());
    }
    std::cout << temp_rate;
    ros::Rate looprate(temp_rate);

    while(ros::ok())
    {
        _pub.publish(_output);
        ros::spinOnce();
        looprate.sleep();
    }

    return 0;
}
