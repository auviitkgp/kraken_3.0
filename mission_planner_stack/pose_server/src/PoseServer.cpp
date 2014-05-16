#include <pose_server/PoseServer.h>

namespace kraken_core
{
  
  PoseServer::PoseServer(Estimator *estimator):_estimator(estimator)
  {
    _good_sensor = _fast_sensor = false;
    ros::NodeHandle n;
    _imu = n.subscribe<kraken_msgs::imuData>("/kraken/imu_data",2,&PoseServer::imuCallBack,this);
    _depth = n.subscribe<kraken_msgs::depthData>("/kraken/depth",2,&PoseServer::depthCallBack,this);
    //_dvl = n.subscribe<kraken_msgs::dvlData>("",2,&PoseServer::dvlCallBack,this);
    _pose = n.advertise<kraken_msgs::krakenPose>("/kraken/pose_estimated",2);
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
    else if(_fast_sensor)
    {
      _estimator->updatePose(_imuData,_depthData);
    }
    kraken_msgs::krakenPose pose;
    float *_data = _estimator->getNextPose().getData();
    for(int i=0;i<countState;i++)
    {
        pose.data[i] = _data[i];
    }
    _pose.publish(pose);
  }
  
  void PoseServer::depthCallBack(const kraken_msgs::depthData::ConstPtr &msg)
  {
    boost::mutex::scoped_lock lock(io_mutex);
    _depthData.depth = msg->depth;
  }
  
  void PoseServer::imuCallBack(const kraken_msgs::imuData::ConstPtr &msg)
  {
    boost::mutex::scoped_lock lock(io_mutex);
    _imuData = *msg;
    _fast_sensor = true;
  }
  
  void PoseServer::dvlCallBack(const kraken_msgs::dvlData::ConstPtr &msg)
  {
    boost::mutex::scoped_lock lock(io_mutex);
    _dvlData = *msg;
    _good_sensor = true;
  }
  
} // end kraken_core

