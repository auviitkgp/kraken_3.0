#ifndef FORCECONVERTER_H
#define FORCECONVERTER_H
#include <ros/ros.h>
#include <kraken_msgs/thrusterData4Thruster.h>
#include <kraken_msgs/thrusterData6Thruster.h>
#include <kraken_msgs/forceData4Thruster.h>
#include <kraken_msgs/forceData6Thruster.h>

namespace kraken_simulator
{
  class ForceConverter
  {
    public:
      ForceConverter(float gain=1.0);
      void thrusterData4ThrusterCallback(const kraken_msgs::thrusterData4Thruster::ConstPtr &msg);
      void thrusterData6ThrusterCallback(const kraken_msgs::thrusterData6Thruster::ConstPtr &msg);
    private:
      ros::Publisher _thrus4_pub;
      ros::Publisher _thrus6_pub;
      ros::Subscriber _thrus4_sub;
      ros::Subscriber _thrus6_sub;
      float _gain;
    protected:
  };
}

#endif // FORCECONVERTER_H
