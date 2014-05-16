#ifndef POSESERVER_H
#define POSESERVER_H
#include <pose_server/Estimator.h>
#include <kraken_msgs/krakenResetPose.h>
#include <kraken_msgs/krakenPose.h>
#include <ros/timer.h>
#include <ros/ros.h>
#include <boost/thread/mutex.hpp>

namespace kraken_core
{
  
  class PoseServer
  {
    private:
      Estimator* _estimator;
      bool _good_sensor;
      bool _fast_sensor;
      kraken_msgs::depthData _depthData;
      kraken_msgs::imuData _imuData;
      kraken_msgs::dvlData _dvlData;
      ros::Subscriber _imu;
      ros::Subscriber _depth;
      ros::Subscriber _dvl;
      ros::Publisher _pose;
      ros::Timer _timer;
      boost::mutex io_mutex;
    public:
      PoseServer(Estimator * estimator);
      bool resetPosition(kraken_msgs::krakenResetPose::Request  &req,
                         kraken_msgs::krakenResetPose::Response &res);
      void timerCallBack(const ros::TimerEvent&);
      void dvlCallBack(const kraken_msgs::dvlData::ConstPtr &msg);
      void depthCallBack(const kraken_msgs::depthData::ConstPtr &msg);
      void imuCallBack(const kraken_msgs::imuData::ConstPtr &msg);
  };
  
} // end kraken_core

#endif // POSESERVER_H
