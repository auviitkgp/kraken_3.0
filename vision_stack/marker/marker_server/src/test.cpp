#include <ros/ros.h>
#include <marker_server/marker_server.h>

int main(int argc, char ** argv)
{
    ros::init(argc, argv, "markerserver");
    Marker _markerAction("marker");
    ros::spin();
    return 0;
}