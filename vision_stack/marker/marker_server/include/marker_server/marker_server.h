#ifndef MARKER_H
#define MARKER_H

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <actionmsg/markerAction.h>
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
#include <marker_server/taskheader.h>
#include <marker_server/resultheader.h>

using namespace std;
using namespace cv;

typedef actionlib::SimpleActionServer<actionmsg::markerAction> Server;

class Marker
{
private:
    ros::NodeHandle _n;
    Server _s;
    image_transport::ImageTransport _it;
    image_transport::Subscriber _sub;
    image_transport::Publisher _pub;
    actionmsg::markerFeedback _feedback;
    actionmsg::markerResult _result;
    Mat I, I_hsv, I_bw;
    Scalar _lowerThresh, _upperThresh;
    Mat _kernelDilateErode;
    vector<RotatedRect> _minRect;
    std::string _actionName;
    cv_bridge::CvImage _finalImage;
    sensor_msgs::ImagePtr _finalImagemsg;

public:
    Marker(std::string _name);
    void executCB(const actionmsg::markerGoalConstPtr &_goal);
    void imageCallBack(const sensor_msgs::ImageConstPtr &msg);
    void detectMarker();
    void getAllignment();
    ~Marker();
};

#endif
