#ifndef VALIDATION_GATE
#define VALIDATION_GATE
 
#define X_MIN 700
#define Y_MIN 700

#include <bits/stdc++.h>
#include <ros/ros.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <opencv/cv.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <actionlib/server/simple_action_server.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <ip_msgs/vgateAction.h>
#include <resources/topicHeader.h>
#include <fstream>
#include <blob/blob.h>
#include <blob/BlobContour.h>
#include <blob/BlobLibraryConfiguration.h>
#include <blob/BlobOperators.h>
#include <blob/BlobProperties.h>
#include <blob/BlobResult.h>
#include <blob/ComponentLabeling.h>


using namespace cv;
using namespace ros;
using namespace std;



typedef actionlib::SimpleActionServer<ip_msgs::vgateAction> Server;



/**
 class to detect the validation gate and align the vehicle
 to calculate the error in location of center of gate w.r.t center of image
 The class uses blob library to filter noise
 */


struct bunch{
    Point p;
    vector<Vec4i> v;
};

Point getMeanOfModes(vector<Point> &);
std::vector<Point> getModePoints(vector<Point>& v);

class Gateserver
{

private:

    Mat _img,_hsv,_gray,_fimg, _kernel,I2,_dilateImg,_thresh,_median;
    Scalar _lowval, _highval;
    VideoCapture _cap;
    NodeHandle _n;
    ip_msgs::vgateFeedback _feed;
    ip_msgs::vgateResult _res;
    std_msgs::Header _h;
    image_transport::ImageTransport _it;
    Server _serv;
    image_transport::Publisher _pub;
    image_transport::Subscriber _imgsub;
    cv_bridge::CvImagePtr bridge_ptr;
    IplImage _fimage;
    int _x1,_x2,_y1,_y2,_index_min,_index_max,_xmin, _ymin, xmin, xmax, ymin, ymax,x1,y1,x2,y2;
    Point2f _center,_centerImg,_error,_pt;
    vector<Point2f> _centerLine;
    float area,angle1,angle2;
    vector<Vec4i>  _lines;
    double _threshold1, _threshold2,_epsilon,_errorAngle;
    vector<vector<Point> > _contours;
    CBlobResult _blobs1,_blobs2;
    CBlob * _currentBlob1,*_currentBlob2,cb;
    pair<bunch,pair<bunch,bunch> > retpair;
    bunch b;
    vector<Vec4i> v;
    bool _gateStatus, _status;
    int index,_THRES_LINE_GAP;
    deque<Point> q;
    Point prevmed, cog, cof;


public:

    //! initializes  calls imageCallback to subscribe the image from front cam
    //! reads threshold value from a text file
    //! starts the server
    Gateserver(NodeHandle &);


    //! Draws a line joining the extreme end points of the gate
    //! publish image data on topic validation gate
    bool detectGate();
    Point getCenterOfGate();
    Point getCenterOfFrame();

    //! Calculates the center of the line joining extreme points
    //! error between the center of line and center of image
    void alignGate();


    pair<bunch,pair<bunch,bunch> > getLargestBunch(const vector<Vec4i> &lines);
    void printbunch( vector<bunch> &b, string name);
    inline bool withinThres(int th,int a ,int b);

    void serverCallback(const ip_msgs::vgateGoalConstPtr &goal);


    //! callback function to subscribe the image published from front cam topic to vaidation gate..
    void imageCallback(const sensor_msgs::ImageConstPtr& ptr);

    //! destructor
    ~Gateserver();
};



#endif VALIDATION_GATE
