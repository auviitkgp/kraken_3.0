#include <iostream>
#include <ros/ros.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

using namespace std;
using namespace cv;

Mat _image;

void imageCallBack(const sensor_msgs::ImageConstPtr &_msg)
{
    cv_bridge::CvImagePtr _imagePtr;
    try
    {
        _imagePtr = cv_bridge::toCvCopy(_msg, "bgr8");
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", _msg->encoding.c_str());
    }
    _image = _imagePtr->image;
}

int main(int argc, char ** argv)
{
	ros::init(argc, argv, "image_subscribe");
    ros::NodeHandle _n;
    image_transport::ImageTransport _it(_n);
    image_transport::Subscriber _sub = _it.subscribe("frontcamimage", 1, imageCallBack);

    ros::Rate _looprate(10);

    while(ros::ok())
    {
        _looprate.sleep();
        ros::spinOnce();
        if(!_image.empty())
            imshow("Image", _image);
        if(waitKey(33) == 27)
            break;
    }

	return 0;
}
