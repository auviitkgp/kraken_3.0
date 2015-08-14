#include <ros/ros.h>
#include "controller_basic/controllerConfig.h"
int main(int argc,char** argv)
{
    ros::init(argc,argv,"Controller");


    ROS_INFO("Spinning node");
    ros::spin();
    return 0;
}
