/**
 * @file /include/App/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef App_QNODE_HPP_
#define App_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <string>
#include <QThread>
#include <QStringListModel>
#include <kraken_msgs/krakenPose.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/Image.h>
#include <kraken_msgs/thrusterData4Thruster.h>
#include <kraken_msgs/thrusterData6Thruster.h>
#include <cv_bridge/cv_bridge.h>
#include <pose_server/KrakenPose.h>
#include <cv.h>
#include "resources/topicHeader.h"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace Telemetry {
  
  const double pi = 3.14159;

/*****************************************************************************
** Class
*****************************************************************************/

  const std::string  _ros_topic_front_image=topics::CAMERA_FRONT_RAW_IMAGE;
  const std::string  _ros_topic_bottom_image=topics::CAMERA_BOTTOM_RAW_IMAGE;
  const std::string  _ros_topic_thruster_6_data=topics::CONTROL_PID_THRUSTER6;
  const std::string  _ros_topic_thruster_4_data=topics::CONTROL_PID_THRUSTER4;
  const std::string  _ros_topic_kraken_current_pose=topics::NAV_POSE_ESTIMATED;
  const std::string  _ros_topic_kraken_set_pose=topics::RESET_POSITION_SERVICE;
  
class QNode : public QThread {
    Q_OBJECT
public:
	QNode(int argc, char** argv );
	virtual ~QNode();
	bool init();
	void run();
	void callBackFrontImage(const sensor_msgs::ImageConstPtr &msg);
	void callBackBottomImage(const sensor_msgs::ImageConstPtr &msg);
	void callBackThruster6Data(const kraken_msgs::thrusterData6ThrusterConstPtr &msg);
	void callBackThruster4Data(const kraken_msgs::thrusterData4ThrusterConstPtr &msg);
	void callBackKrakenCurrentPose(const kraken_msgs::krakenPoseConstPtr &msg);
	void callBackKrakenSetPose(const kraken_msgs::krakenPoseConstPtr &msg);
Q_SIGNALS:
    void rosShutdown();
    void updateCurrentPose(kraken_msgs::krakenPoseConstPtr msg);
    void updateSetPose(kraken_msgs::krakenPoseConstPtr msg);
    void updateFrontImage(sensor_msgs::ImageConstPtr _msg);
    void updateBottomImage(sensor_msgs::ImageConstPtr _msg);

private:
	int init_argc;
	char** init_argv;
	ros::Subscriber _ros_subscriber_front_image;
	ros::Subscriber _ros_subscriber_bottom_image;
	ros::Subscriber _ros_subscriber_thruster_4_data;
	ros::Subscriber _ros_subscriber_thruster_6_data;
	ros::Subscriber _ros_subscriber_kraken_current_pose;
	ros::Subscriber _ros_subscriber_set_pose;
};

}  // namespace App

#endif /* App_QNODE_HPP_ */
