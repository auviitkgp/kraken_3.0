#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <std_msgs/String.h>

#include <resources/topicHeader.h>

class Camera {
public:
  Camera(int camNo=0);
  cv::VideoCapture cam;
  int cameraNo;
  bool camOpen;

  // Getter functions

  cv::VideoCapture returnCamera();
  int getCurrentCameraNo();
  bool isOpen();

  // Setter functions

  void switchCamera();
  void setCameraState(bool);
  void setCameraNo(int camNo);

  // Functions to print the object

  void inspect();
};
