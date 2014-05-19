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

    Point2f *MARKERS;
    int _nMarkers;
    void initMap();
    Mat image;

    Point2f _vehicle_Pos;
    NodeHandle &_n;
    ServiceServer _server;
    Map(NodeHandle &,Point2f*,int);
    ~Map(){}
    void updateVehiclePosition(const geometry_msgs::PoseConstPtr &);
    bool getObjectLocation(premap::getLocation::Request &req, premap::getLocation::Response &res);

private:
    void updateVehiclePosition(const Point2f &);
    float roundOff(float);
    void showMap();
    void updateMapVideo(Point &);
};





