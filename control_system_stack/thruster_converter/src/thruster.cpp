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
#include <iostream>
#include <stdlib.h>
#include <ros/ros.h>
#include <kraken_msgs/seabotix.h>
#include <kraken_msgs/thrusterData4Thruster.h>
#include <kraken_msgs/thrusterData6Thruster.h>
#include <SerialPort/SerialPort.h>
#include <resources/topicHeader.h>

float converter = 1.0;
uint8_t offset = 0x80;
uint8_t max = 0xCD;   //Maximum forward thrust
uint8_t min = 0x32;   //Maximum backward thrust
kraken_msgs::seabotix _output;

void thruster4callback(const kraken_msgs::thrusterData4ThrusterConstPtr &msg)
{
    float inData[4] = {0,0,0,0};
    uint8_t store;


    for(int j=0; j<6 ; j++)
        _output.data[j] = offset;

    for(int i = 0; i<4 ; i++ )
    {
        inData[i] = msg->data[i];
	ROS_INFO("indata[%d] : %f",i,inData[i]);
        store = uint8_t((converter*inData[i]>0xCD-0x80?(0xCD-0x80):converter*inData[i])+offset);
	store = uint8_t((converter*inData[i]<0x32-0x80?(0x32-0x80):converter*inData[i])+offset);
		// OFFSET - 1 FOR REVERSE SPEED (0 SPEED FOR REVERSE THRUST = 0X7f = 0X80 - 1) 
	ROS_INFO("store : %d",store);
        if (store > max)
            store = max;

        if (store < min)
            store = min;

        _output.data[i] = store;
    }


}

void thruster6callback(const kraken_msgs::thrusterData6ThrusterConstPtr msg)
{

/*
HOTFIX by Aniket in May, 2015

Due to thruster remapping, the if else constructs
have to be introduced here, so as to ensure that
the other packages that depend on this convention:

thruster data array[4] -> surge left
thruster data array[5] -> surge right

*/
    float inData[6] = {0.0,0.0,0.0,0.0,0.0,0.0};
    uint8_t store;


    for(int j=0; j<6 ; j++)
        _output.data[j] = offset;

    for(int i = 0; i<6 ; i++ )
    {
        inData[i] = msg->data[i];
	ROS_INFO("indata[%d] : %f",i,inData[i]);
        store = uint8_t((converter*inData[i]>(0xCD-0x80)?(0xCD):converter*inData[i]+offset));
	store = uint8_t((converter*inData[i]<(0x32-0x7F)?(0x32):converter*inData[i]+offset-1));
	ROS_INFO("store : %d",store);
        if (store > max)
            store = max;

        if (store < min)
            store = min;

	if(i == 4)

		_output.data[1] = store;

	else

		if (i == 5)

			_output.data[4] = store;

		else

			_output.data[i] = store;
 
   }

}

int main(int argc,char** argv)
{

    ros::init(argc ,argv, "seabotixConverter");

    ros::NodeHandle n;
    ros::Subscriber _sub4 = n.subscribe<kraken_msgs::thrusterData4Thruster>(topics::CONTROL_PID_THRUSTER4,1,thruster4callback);
    ros::Subscriber _sub6 = n.subscribe<kraken_msgs::thrusterData6Thruster>(topics::CONTROL_PID_THRUSTER6,1,thruster6callback);
    ros::Publisher _pub = n.advertise<kraken_msgs::seabotix>(topics::CONTROL_SEABOTIX,1);

//    Serial arduino;


    ros::Rate looprate(10);
    
    while(ros::ok())
    {
        _pub.publish(_output);
        ros::spinOnce();
        looprate.sleep();
    }

return 0;
}

