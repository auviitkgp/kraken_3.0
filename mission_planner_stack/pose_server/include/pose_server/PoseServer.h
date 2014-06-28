#ifndef POSESERVER_H
#define POSESERVER_H
#include <pose_server/Estimator.h>
#include <kraken_msgs/krakenResetPose.h>
#include <kraken_msgs/krakenPose.h>
#include <ros/timer.h>
#include <ros/ros.h>
#include <boost/thread/mutex.hpp>
#include "sensor_msgs/Imu.h"
#include <tracks_imu/Tracks.h>
#include <tf/tf.h>
#include <kdl/kdl.hpp>
#include <bullet/LinearMath/btMatrix3x3.h>
#include <bullet/LinearMath/btQuaternion.h>
#include <underwater_sensor_msgs/Pressure.h>
#include <math.h>

namespace kraken_core
{
  const double pi = 3.1415926;
  class PoseServer
  {
    private:
      /*
       * abstract Estimator. Can be different type of estimators
       */
      Estimator* _estimator;
      /*
       * Has data from accurate sensor has arrived
       */
      bool _good_sensor;
      /*
       * Has data from faster sensor has arrived
       */
      bool _fast_sensor;

      /*
       * Has data from depth sensor has arrived
       */
      bool _depth_sensor;
      /*
       * Datastructure for depth data
       */
      kraken_msgs::depthData _depthData;
      /*
       * Datastructure for IMU data
       */
      kraken_msgs::imuData _imuData;
      /*
       * Datastructure for DVL data
       */
      kraken_msgs::dvlData _dvlData;
      /*
       * Subscribers for different sensors
       */
      ros::Subscriber _imu_sub;
      ros::Subscriber _depth_sub;
      ros::Subscriber _dvl_sub;
      /*
       * Publisher of position
       */
      ros::Publisher _pose_pub;
      /*
       * Timer
       */
      ros::Timer _timer;
      /*
       * Lock for syncronization
       */
      boost::mutex io_mutex;
    public:
      /*
       * Constructor takes an estimator
       */
      PoseServer(Estimator * estimator);
      /*
       * function to reset server using service
       */
      bool resetPosition(kraken_msgs::krakenResetPose::Request  &req,
                         kraken_msgs::krakenResetPose::Response &res);
      /*
       * Timer Callback
       */
      void timerCallBack(const ros::TimerEvent&);
      /*
       * DVL Callback
       */
      void dvlCallBack(const kraken_msgs::dvlData::ConstPtr &msg);
      /*
       * Depth Callback with different input data type
       */
      //void depthCallBack(const kraken_msgs::depthData::ConstPtr &msg);
      void depthCallBack(const underwater_sensor_msgs::Pressure::ConstPtr &msg);
      /*
       * IMU Callback with different input data type
       */
      //void imuCallBack(const sensor_msgs::Imu::ConstPtr &msg);
      void imuCallBack(const kraken_msgs::imuData::ConstPtr &msg);
  };
  
} // end kraken_core

#endif // POSESERVER_H
