#include <ros/ros.h>
#include <thruster_force_converter/ForceConverter.h>


int main(int argc,char** argv)
{
  ros::init(argc,argv,"converter");
  float gain = 0.5;
  if(argc==2)
    {
      gain = atof(argv[1]);
    }
  else
    {
      ROS_INFO("Please give the gain as first parameter");
      exit(0);
    }
  kraken_simulator::ForceConverter converter(gain);
  ros::spin();
  return 0;
}
