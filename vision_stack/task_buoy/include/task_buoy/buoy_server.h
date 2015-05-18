#ifndef BUOY_H
#define BUOY_H

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <actionmsg/buoyAction.h>
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
#include <task_buoy/taskheader.h>
#include <task_buoy/resultheader.h>

using namespace std;
using namespace cv;

typedef actionlib::SimpleActionServer<actionmsg::buoyAction> Server;


class Buoy
{
private:
    ros::NodeHandle _n;
    Server _s;
    image_transport::ImageTransport _it;
    image_transport::Subscriber _sub;
    image_transport::Publisher _pub;
    actionmsg::buoyFeedback _feedback;
    actionmsg::buoyResult _result;
    
    ros::Publisher _buoy_coord_pub;
    ros::NodeHandle _nh;

    Mat _image, _imageHSV, _imageBW, _imageBWRed, _imageBWGreen;
    Scalar _lowerThreshRed1, _lowerThreshRed2, _upperThreshRed1, _upperThreshRed2;
    Scalar _lowerThreshGreen, _upperThreshGreen, _lowerThreshYellow, _upperThreshYellow;
    Mat _kernelDilateErode;
    std::string _actionName;
    cv_bridge::CvImage _finalImage;
    sensor_msgs::ImagePtr _finalImagemsg;
    vector<vector<Point> > _contours;
    vector<vector<Point> > _contoursPoly;
    vector<Point2f> _center;
    vector<float> _radius;
    vector<Vec3f> circles;
    vector<int> _fradius;
    vector<Point> _fcenter;
    int max ;
    int index;
public:
    Buoy(std::string _name);
    void executeCB(const actionmsg::buoyGoalConstPtr &_goal);
    void imageCallBack(const sensor_msgs::ImageConstPtr &msg);
    float angleWrtY(const Point2f &v1, const Point2i &v2);
    Point2i rotatePoint(const Point2i &v1, const Point2i &v2, float angle);
    bool detectBuoy();
    void getAllignment();
    ~Buoy();
};

#endif

