#include <pose_server/PoseServer.h>
#include <pose_server/DeadReckoning.h>
#include "resources/topicHeader.h"
int main(int argc,char** argv)
{
  ros::init(argc,argv,"PoseServer");
  ros::NodeHandle n;
  kraken_core::DeadReckoning estimator(2,0.05);
  kraken_core::PoseServer server(&estimator);
  ros::ServiceServer service = n.advertiseService(topics::RESET_POSITION_SERVICE,&kraken_core::PoseServer::resetPosition,&server);
  ROS_INFO("POSE SERVER STARTED");
  ros::spin();
  
  return 0;
}
