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
  Camera(int);
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

Camera::Camera (int cameraNo=0) {
  std::cout << "The object has been created.";
  this->cameraNo = 0;
  this->camOpen = false;
}

cv::VideoCapture Camera::returnCamera () {
  return this->cam;
}

int Camera::getCurrentCameraNo () {
  return cameraNo;
}

bool Camera::isOpen () {
  return camOpen;
}

void Camera::switchCamera () {
  if (this->cameraNo == 0) {
    this->cameraNo = 1;
  } else {
    this->cameraNo = 0;
  }
}

void Camera::setCameraState (bool state) {
  this->camOpen = state;
}

void Camera::setCameraNo (int camNo) {
  this->cameraNo = camNo;
}

void Camera::inspect () {
  std::cout << "The current camera is: " << this->getCurrentCameraNo() << "\n";
  std::cout << "The current camera state is: " << this->isOpen() << "\n";
}
