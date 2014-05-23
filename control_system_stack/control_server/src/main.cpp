#include<ros/ros.h>
#include <control_server/ControlServer.h>


int main(int argc,char** argv)
{
  ros::init(argc,argv,"Control_Server");
  ros::NodeHandle n;
  kraken_controller::ControlServer _server(10);
  actionlib::SimpleActionServer<kraken_msgs::advancedControllerAction> _ser1(n,ros::this_node::getName(),boost::bind(&kraken_controller::ControlServer::executePoseChange, &_server, _1),false);
  actionlib::SimpleActionServer<kraken_msgs::controllerAction> _ser2(n,ros::this_node::getName(),boost::bind(&kraken_controller::ControlServer::executeOrientationChange, &_server, _1),false);
  ros::ServiceServer service1 = n.advertiseService("/kraken/translate_auv",&kraken_controller::ControlServer::moveAlongLine,&_server);
  ros::ServiceServer service2 = n.advertiseService("/kraken/switch_controller",&kraken_controller::ControlServer::moveAlongLine,&_server);
  _server.setServers(&_ser1,&_ser2);
  ros::spin();  
  return 0;
}
