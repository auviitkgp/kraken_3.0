#include "bottomcam/bottomcam.h"

Camera::Camera (int cameraNo=0) {
  std::cout << "The object has been created.";
  this->cameraNo = 0;
  this->camOpen = false;
}

cv::VideoCapture Camera::returnCamera () {
  return this->cam;
}

int Camera::getCurrentCamera () {
  return cameraNo;
}

bool Camera::getCameraState () {
  return camOpen;
}
