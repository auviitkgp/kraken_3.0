#include <pose_server/PoseServer.h>
#include <pose_server/DeadReckoning.h>
#include "resources/topicHeader.h"
#include "pose_server/KalmanEstimator.h"
#include "pose_server/Estimator.h"
int main(int argc,char** argv)
{
    ros::init(argc,argv,"PoseServer");
    ros::NodeHandle n;
    kraken_core::Estimator* est=NULL;

    if(1)

    {
//      kraken_core::KalmanEstimator estimator();
        est=new kraken_core::KalmanEstimator();
    }
    else
    {
//    kraken_core::DeadReckoning estimator(2,0.05);
        est=new kraken_core::DeadReckoning();
    }

    kraken_core::PoseServer server(est);
    ros::ServiceServer service = n.advertiseService(topics::RESET_POSITION_SERVICE,&kraken_core::PoseServer::resetPosition,&server);
    ROS_INFO("POSE SERVER STARTED");
    ros::spin();
    return 0;
}
