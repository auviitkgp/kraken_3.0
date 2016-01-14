#include <iostream>
#include <ros/ros.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

using namespace std;
using namespace cv;

int main(int argc, char ** argv)
{
    ros::init(argc, argv, "videoread");

    if(argc != 3)
    {
        cout << "videoread : Requires 1) video file path, 2)target topic name as arguments." << endl;
        ros::shutdown();
    }

    std::string _videopath = argv[1];
    std::string _topicname = argv[2];

    ros::NodeHandle _n;
    image_transport::ImageTransport _it(_n);
    image_transport::Publisher _pub = _it.advertise(_topicname, 1);
    sensor_msgs::ImagePtr _publishImage;
    cv_bridge::CvImage _image;

    double temp_rate;
//  std::string string_rate;
    if (_n.hasParam("/ros_rate"))
    {
        _n.getParam("/ros_rate", temp_rate);
//      temp_rate = atof(string_rate.c_str());
    }
    else
    {
        std::runtime_error::runtime_error("ROSParam '/ros_rate' does not exist.");
        ROS_ERROR("ROSParam '/ros_rate' does not exist.");
        ros::shutdown();
        return ;

    }
    ros::Rate _looprate(temp_rate);

    VideoCapture _camera(_videopath.c_str());

    if(_camera.isOpened())
    {
        ROS_INFO("Video file opened successfully");
    }
    else
    {
        ROS_ERROR("Video file not opened.");
        ros::shutdown();
    }

    _image.encoding = "bgr8";

    while(ros::ok())
    {
        _camera >> _image.image;
        _publishImage = _image.toImageMsg();
        _pub.publish(_publishImage);
        _looprate.sleep();
        ros::spinOnce();
    }

    return 0;
}
