#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "ros/ros.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Pose.h"
#include "iostream"
#include "premap/getLocation.h"
using namespace std;
using namespace cv;
using namespace ros;
class Map{
public:
    //stores Markers points for each task MARKER[0] gives marker position for task1
    Point2f *MARKERS;
    //total number of markers
    int _nMarkers;

    void initMap();
    //stores the basic map with all markers as OpenCv Mat
    Mat image;
    //the current position of the vehicle
    Point2f _vehicle_Pos;

    NodeHandle &_n;
    ServiceServer _server;
    Map(NodeHandle &,Point2f*,int);
    ~Map(){}
    //Callback from subsricption to /kraken/estimated_pose
    void updateVehiclePosition(const geometry_msgs::PoseConstPtr &);

    //Service to get Location of any marker or object that is placed within
    bool getObjectLocation(premap::getLocation::Request &req, premap::getLocation::Response &res);

private:
    void updateVehiclePosition(const Point2f &);
    float roundOff(float);
    void showMap();
    void updateMapVideo(Point &);
};





