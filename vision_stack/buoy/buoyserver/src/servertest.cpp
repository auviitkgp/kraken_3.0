#include <ros/ros.h>
#include <buoyserver/buoyserver.h>

int main(int argc, char ** argv)
{
    ros::init(argc, argv, "buoyserver");
    Buoy _buoyserver("buoy");
    ros::spin();
    return 0;
}
