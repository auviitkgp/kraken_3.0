#include <pose_server/PoseServer.h>
#include "resources/topicHeader.h"
namespace kraken_core
{
  
  PoseServer::PoseServer(Estimator *estimator):_estimator(estimator)
  {
    _good_sensor = _fast_sensor = false;
    ros::NodeHandle n;
    _imu_sub = n.subscribe<kraken_msgs::imuData>(topics::SENSOR_IMU,1,&PoseServer::imuCallBack,this);
    //_imu = n.subscribe<sensor_msgs::Imu>("/kraken/imuData",2,&PoseServer::imuCallBack,this);
    _depth_sub = n.subscribe<underwater_sensor_msgs::Pressure>(topics::SENSOR_DEPTH,1,&PoseServer::depthCallBack,this);
    //_depth = n.subscribe<kraken_msgs::depthData>("/kraken/depth",2,&PoseServer::depthCallBack,this);
    _dvl_sub = n.subscribe<kraken_msgs::dvlData>(topics::SENSOR_DVL,2,&PoseServer::dvlCallBack,this);
    _pose_pub = n.advertise<kraken_msgs::krakenPose>(topics::NAV_POSE_ESTIMATED,1);
    _timer = n.createTimer(ros::Duration(estimator->getTime()),&PoseServer::timerCallBack,this);
  }
  
  bool PoseServer::resetPosition(kraken_msgs::krakenResetPose::Request &req, kraken_msgs::krakenResetPose::Response &res)
  {
    KrakenPose pose;
    float * data = pose.getData();
    for(int i=0;i<countState;i++)
    {
        data[i] = req.pose.data[i];
    }
    //std::cerr<<"Service Requested";
    boost::mutex::scoped_lock lock(io_mutex);
    _estimator->resetPose(pose);
    return true;
  }
  
  void PoseServer::timerCallBack(const ros::TimerEvent &)
  {
    boost::mutex::scoped_lock lock(io_mutex);
    if(_good_sensor)
    {
      _estimator->updatePose(_imuData,_depthData,_dvlData);
      _good_sensor = false;
    }
    else if(_fast_sensor && _depth_sensor)
    {
      _estimator->updatePose(_imuData,_depthData);
      _fast_sensor = false;
      _depth_sensor=false;
    }
    else if(_fast_sensor)
      {
        _estimator->updatePose(_imuData);
        _fast_sensor = false;
        return ;
      }
    kraken_msgs::krakenPose pose;
    float *_data = _estimator->getNextPose().getData();
    for(int i=0;i<countState;i++)
    {
        pose.data[i] = _data[i];
    }
    _pose_pub.publish(pose);
  }
  void PoseServer::depthCallBack(const underwater_sensor_msgs::Pressure::ConstPtr &msg)
  //void PoseServer::depthCallBack(const kraken_msgs::depthData::ConstPtr &msg)
  {
    boost::mutex::scoped_lock lock(io_mutex);
    //_depthData.depth = msg->depth;
    _depthData.depth = msg->pressure;
    _depth_sensor=true;
  }
  
  void PoseServer::imuCallBack(const kraken_msgs::imuData::ConstPtr &msg)
  {
    boost::mutex::scoped_lock lock(io_mutex);
    // convert data to our format
    for(int i=0;i<13;i++)
    {
        _imuData.data[i] = msg->data[i];
    }
    //_imuData.data[kraken_sensors::yaw]+=180;
    _fast_sensor = true;
  }
  
  /*void PoseServer::imuCallBack(const sensor_msgs::Imu::ConstPtr &msg)
  {
    boost::mutex::scoped_lock lock(io_mutex);
    // convert data to our format
    _imuData.data[kraken_sensors::accelX] = msg->linear_acceleration.x;
    _imuData.data[kraken_sensors::accelY] = msg->linear_acceleration.y;
    _imuData.data[kraken_sensors::accelZ] = msg->linear_acceleration.z;
    btMatrix3x3(btQuaternion(msg->orientation.x,msg->orientation.y,
                             msg->orientation.z,msg->orientation.w)).getEulerYPR(
          _imuData.data[kraken_sensors::yaw],_imuData.data[kraken_sensors::pitch],_imuData.data[kraken_sensors::roll]);
    _imuData.data[kraken_sensors::yaw]+=180;
    //_imuData.data[kraken_sensors::roll]+=180;
    //_imuData.data[kraken_sensors::pit]+=180;
    _fast_sensor = true;
  }
  */
  void PoseServer::dvlCallBack(const kraken_msgs::dvlData::ConstPtr &msg)
  {
    boost::mutex::scoped_lock lock(io_mutex);
    _dvlData = *msg;
    _good_sensor = true;
  }
  
} // end



