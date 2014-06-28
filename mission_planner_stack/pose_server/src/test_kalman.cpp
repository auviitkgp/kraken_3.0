#include <pose_server/PoseServer.h>
#include <pose_server/DeadReckoning.h>
#include <pose_server/KalmanEstimator.h>

void imuCallBack(const kraken_msgs::imuDataConstPtr& msg)
{
    float ax=msg->data[kraken_sensors::accelX];
    float ay=msg->data[kraken_sensors::accelY];
    float az=msg->data[kraken_sensors::accelZ];
    float r=msg->data[kraken_sensors::gyroX];
    float p=msg->data[kraken_sensors::gyroY];
    float y=msg->data[kraken_sensors::gyroZ];


    Matrix3d RWB;
    RWB<<cos(p)*cos(y),     sin(r)*sin(p)*cos(y)-cos(r)*sin(y),            cos(r)*sin(p)*cos(y)+sin(r)*sin(y),
            cos(p)*sin(y),   sin(r)*sin(p)*sin(y)+cos(r)*cos(y),           cos(r)*sin(p)*sin(y)-sin(r)*cos(y),
            -sin(p)  ,        sin(r)*cos(y),                                 cos(r)*cos(p);

    Vector3d Aw=RWB*Vector3d(ax,ay,az);
    ROS_INFO("Body---->  %lf %lf %lf",ax,ay,az);
    ROS_INFO("world---->  %lf %lf %lf",Aw(0),Aw(1),Aw(2));

}

int main(int argc,char** argv)
{
  ros::init(argc,argv,"PoseServer");
  ros::NodeHandle n;
  kraken_core::KalmanEstimator estimator(2,0.05);
  kraken_core::PoseServer server(&estimator);
  ros::ServiceServer service = n.advertiseService("kraken_pose_change",&kraken_core::PoseServer::resetPosition,&server);
  ROS_INFO("POSE SERVER STARTED");
  ros::spin();



//  ros::Subscriber sub = n.subscribe("/kraken/imuData",1,&imuCallBack);
//  ros::spin();
  return 0;
}
