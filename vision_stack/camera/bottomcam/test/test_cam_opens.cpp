// Bring in my package's API, which is what I'm testing
#include "bottomcam/bottomcam.h"
// Bring in gtest
#include <gtest/gtest.h>

// Declare a test
TEST(CameraTest, EnsureCameraConstructorWorks)
{
  Camera _camObj;
  EXPECT_EQ(_camObj.getCurrentCameraNo(), 0);
  EXPECT_EQ(_camObj.isOpen(), false);
  Camera _camObj_1(1);
  EXPECT_EQ(_camObj_1.getCurrentCameraNo(), 1);
}

TEST(CameraTest, EnsureCameraIsSendingImages)
{
  Camera _camObj;
  cv::VideoCapture _camera = _camObj.returnCamera();
  cv::Mat _frame;
  _camera.open(0);
  _camera >> _frame;
  EXPECT_EQ(_frame.empty(), false);
  EXPECT_GT(_frame.size().height, 0);
  EXPECT_GT(_frame.size().width, 0);
}

// Run all the tests that were declared with TEST()
int main(int argc, char **argv){
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
