#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "ros/ros.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Pose.h"
#include "iostream"
#include "premap/getLocation.h"
#include "kraken_msgs/krakenPose.h"
#include "pose_server/KrakenPose.h"
#include "premap/getLocation.h"
#include "geometry_msgs/Pose.h"
#include "cstdio"
#include "cstring"
#include "opencv2/highgui/highgui.hpp"



namespace premap_module {

#define MAX_X 100
#define MAX_Y 100
#define MAX_Z 100

#define UNKNOWN 0
#define TRAVELLED 1
#define SEEN 2

#define CAM_DEPTH_MAX 4
#define CAMERA_ANGLE .3

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
    Mat _image;
    //the current position of the vehicle
    geometry_msgs::Pose _vehicle_Pos;

    NodeHandle &_node_handle;
    ServiceServer _server;
    Map(NodeHandle &,Point2f*,int);
    ~Map(){}
    //Callback from subsricption to /kraken/estimated_pose
    void updateVehiclePosition(const kraken_msgs::krakenPoseConstPtr &est_pose_msg);

    //Service to get Location of any marker or object that is placed within
    bool getObjectLocation(premap::getLocation::Request &req, premap::getLocation::Response &res);

private:
    int _map_data[MAX_X][MAX_Y][MAX_Z];
    void updateVehiclePosition(const Point3f &);
    float roundOff(float);
    void showMap();
    void updateMapVideo(Point &);

};

}




