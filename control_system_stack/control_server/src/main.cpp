#include<ros/ros.h>
#include <control_server/ControlServer.h>
#include "resources/topicHeader.h"

int main(int argc,char** argv)
{
  ros::init(argc,argv,"Control_Server");
  ros::NodeHandle n;
  if(argc>2)
  {
      std::cerr<<atoi(argv[1])<<std::endl;
      float freq = atof(argv[1]) ;
      kraken_controller::ControlServer _server(freq);
      std::vector<std::string> _files;
      for(int i=2;i<argc;i++)
      {
          _files.push_back(argv[i]);
      }
      _server.loadParams(_files);
      actionlib::SimpleActionServer<kraken_msgs::advancedControllerAction> _ser1(n,topics::CONTROL_ADVANCEDCONTROLLER_ACTION/*ros::this_node::getName()*/,boost::bind(&kraken_controller::ControlServer::executePoseChange, &_server, _1),false);
      actionlib::SimpleActionServer<kraken_msgs::controllerAction> _ser2(n,topics::CONTROL_SETPOINT_ACTION/*ros::this_node::getName()*/,boost::bind(&kraken_controller::ControlServer::executeOrientationChange, &_server, _1),false);
      ros::ServiceServer service1 = n.advertiseService(topics::CONTROL_MOVEALONG_SERV,&kraken_controller::ControlServer::moveAlongLine,&_server);
      ros::ServiceServer service2 = n.advertiseService(topics::CONTROL_SWITCH_CONTROL,&kraken_controller::ControlServer::changeController,&_server);
      _server.setServers(&_ser1,&_ser2);
      ros::spin();  
  }
  else
  {
      std::cerr<<"server 'freq' 'file1' 'file2' .... " <<std::endl;
  }
  return 0;
}
