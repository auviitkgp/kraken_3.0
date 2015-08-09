#include "bottomcam/bottomcam.h"

Camera::Camera (int cameraNo) {
  std::cout << "The object has been created.";
  this->cameraNo = cameraNo;
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
