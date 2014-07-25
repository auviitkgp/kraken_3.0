#include <thruster_force_converter/ForceConverter.h>

#include <resources/topicHeader.h>

namespace kraken_simulator
{
  ForceConverter::ForceConverter(float gain):_gain(gain)
  {
    ros::NodeHandle n;
    _thrus4_pub = n.advertise<kraken_msgs::forceData4Thruster>(topics::SIMULATOR_MODEL_FORCE_DATA_4_THRUSTERS,1);
    _thrus6_pub = n.advertise<kraken_msgs::forceData6Thruster>(topics::SIMULATOR_MODEL_FORCE_DATA_6_THRUSTERS,1);
    _thrus4_sub = n.subscribe<kraken_msgs::thrusterData4Thruster>(topics::CONTROL_PID_THRUSTER4,1,&ForceConverter::thrusterData4ThrusterCallback,this);
    _thrus6_sub = n.subscribe<kraken_msgs::thrusterData6Thruster>(topics::CONTROL_PID_THRUSTER6,1,&ForceConverter::thrusterData6ThrusterCallback,this);
  }
  
  void ForceConverter::thrusterData4ThrusterCallback(const kraken_msgs::thrusterData4Thruster::ConstPtr &msg)
  {
    kraken_msgs::forceData4Thruster _data;
    _data.data[0] = msg->data[0]*_gain;
    _data.data[1] = msg->data[1]*_gain;
    _data.data[2] = msg->data[2]*_gain;
    _data.data[3] = msg->data[3]*_gain;
    _thrus4_pub.publish(_data);
  }
  
  void ForceConverter::thrusterData6ThrusterCallback(const kraken_msgs::thrusterData6Thruster::ConstPtr &msg)
  {
    kraken_msgs::forceData6Thruster _data;
    _data.data[0] = msg->data[0]*_gain;
    _data.data[1] = msg->data[1]*_gain;
    _data.data[2] = msg->data[2]*_gain;
    _data.data[3] = msg->data[3]*_gain;
    _data.data[4] = msg->data[4]*_gain;
    _data.data[5] = msg->data[5]*_gain;
    _thrus6_pub.publish(_data);
  }
  
}
