#include <ros/ros.h>
#include <NqDVL/NqDVL.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "DVL_NODE");
    ros::NodeHandle n;
    kraken_sensors::NqDVL _myDVL("/dev/ttyUSB0", 115200);
    std::cout<<"starting\n";
    _myDVL.start();
    ros::spin();
    return 0;
}
