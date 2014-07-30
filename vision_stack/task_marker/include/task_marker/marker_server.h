#ifndef MARKER_H
#define MARKER_H

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <ip_msgs/markerAction.h>
#include <actionlib/server/simple_action_server.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <iostream>
#include <fstream>
#include <blob/blob.h>
#include <blob/BlobContour.h>
#include <blob/BlobLibraryConfiguration.h>
#include <blob/BlobOperators.h>
#include <blob/BlobProperties.h>
#include <blob/BlobResult.h>
#include <blob/ComponentLabeling.h>
#include <task_marker/taskheader.h>
#include <task_marker/resultheader.h>

using namespace std;
using namespace cv;

typedef actionlib::SimpleActionServer<ip_msgs::markerAction> Server;

class Marker
{
private:
    ros::NodeHandle _n;
    Server _s;
    image_transport::ImageTransport _it;
    image_transport::Subscriber _sub;
    image_transport::Publisher _pub;
    ip_msgs::markerFeedback _feedback;
    ip_msgs::markerResult _result;
    Mat I, I_hsv, I_bw;
    Scalar _lowerThresh, _upperThresh;
    Mat _kernelDilateErode;
    vector<RotatedRect> _minRect;
    std::string _actionName;
    cv_bridge::CvImage _finalImage;
    sensor_msgs::ImagePtr _finalImagemsg;


    bool marker_detect_status;
    bool marker_align_status;
public:
    Marker(std::string _name, std::string _threshold_filepath);
    void executeCB(const ip_msgs::markerGoalConstPtr &_goal);
    void imageCallBack(const sensor_msgs::ImageConstPtr &msg);
    void detectMarker();
    void getAllignment();
    ~Marker();
};

#endif
