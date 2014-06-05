#include <ros/ros.h>
#include <kraken_msgs/krakenResetPose.h>
#include <pose_server/KrakenPose.h>
#include <math.h>
int main(int argc,char** argv)
{
  
    ros::init(argc, argv, "pose_server_client");
  
    ros::NodeHandle n;
    ros::ServiceClient client = n.serviceClient<kraken_msgs::krakenResetPose>("kraken_pose_change");
    kraken_msgs::krakenResetPose srv;
    for(int i=0;i<kraken_core::countState;i++)
    {
      srv.request.pose.data[i] = (rand()%100)/100.0;
      std::cerr<<srv.request.pose.data[i]<<"\t";
    }
    if (client.call(srv))
    {
      ROS_INFO("Sum: %d", srv.response.check);
    }
    else
    {
      ROS_ERROR("ERROR");
      return 1;
    }
  
  return 0;
}
