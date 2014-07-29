#include <control_server/ControlServer.h>
#include <resources/topicHeader.h>
namespace kraken_controller
{
  ControlServer::ControlServer(float freq)
  {
    ros::NodeHandle n;
    _time = n.createTimer(ros::Duration(1.0/freq),&ControlServer::timeCallBack,this);
    _pub = n.advertise<kraken_msgs::thrusterData4Thruster>(topics::CONTROL_PID_THRUSTER4,2);
    _pub6 = n.advertise<kraken_msgs::thrusterData6Thruster>(topics::CONTROL_PID_THRUSTER6,2);
    _set_point = n.advertise<kraken_msgs::krakenPose>("/kraken/pos_set",2);
    _sub_pose = n.subscribe<kraken_msgs::krakenPose>(topics::NAV_POSE_ESTIMATED,2,&ControlServer::poseFeedBack,this);
    _sub_ip_error  = n.subscribe<kraken_msgs::ipControllererror>(topics::CONTROL_IP_ERROR,2,&ControlServer::ipErrorFeedBack,this);

    _do_control = true;
    _ip_controller = false;
    for(int i=0;i<kraken_core::countState;i++)
    {
      _feedBack.data[i] = 0;
    }
  }
  ControlServer::~ControlServer()
  {
    
  }
  
  void ControlServer::timeCallBack(const ros::TimerEvent &)
  {
    //std::cerr<<"calling"<<std::endl;
    if(_do_control)
      {
        _controller.doControlIteration(_feedBack);

        if(_ip_controller)
            _controller.updateIPState();
        else
            _controller.updateState();

        _pub6.publish(_controller.getThruster6Value());
      }
  }
  
  void ControlServer::setServers(actionlib::SimpleActionServer<kraken_msgs::advancedControllerAction> *ser1, actionlib::SimpleActionServer<kraken_msgs::controllerAction> *ser2)
  {
    _server1 = ser1;
    _server2 = ser2;
    _server1->start();
    _server2->start();
  }
  
  void ControlServer::poseFeedBack(const kraken_msgs::krakenPose::ConstPtr &msg)
  {
    
    if(!_ip_controller)
      {
        _feedBack.data[kraken_core::_px] = msg->data[kraken_core::_px];
        _feedBack.data[kraken_core::_py] = msg->data[kraken_core::_py];
        _feedBack.data[kraken_core::_pz] = msg->data[kraken_core::_pz];
      }
    for(int i=kraken_core::_pz+1;i<kraken_core::countState;i++)
      {
        _feedBack.data[i] = msg->data[i];
      }
    
  }
  
  void ControlServer::ipErrorFeedBack(const kraken_msgs::ipControllererror::ConstPtr &msg)
  {
    if(_ip_controller)
      {
        _feedBack.data[kraken_core::_px] = msg->dx;
        _feedBack.data[kraken_core::_py] = msg->dy;
        _feedBack.data[kraken_core::_pz] = msg->dz;
      }
  }
  
  bool ControlServer::moveAlongLine(kraken_msgs::moveAlongLine::Request &req, kraken_msgs::moveAlongLine::Response &res)
  {
    if(req.type==0)
      {
        _controller.pause();
      }
    else
      {
        if(req.angle<=2.0)
          {
            _controller.moveForward();
          }
        else
          {
            _controller.moveBack();
          }
      }
    return true;
  }
  
  bool ControlServer::changeController(kraken_msgs::switchControllers::Request &req, kraken_msgs::switchControllers::Response &res)
  {
    if(req.type==0)
      {
        _ip_controller = false;
      }
    else
      {
        _ip_controller = true;
      }
    return true;
  }
  
  void ControlServer::executePoseChange(const kraken_msgs::advancedControllerGoalConstPtr &msg)
  {
    _do_control = false;

    kraken_msgs::krakenPose _pose;
    kraken_msgs::krakenPose _currPos;
    _controller.getState(_pose);
    _controller.getState(_currPos);
    _pose.data[kraken_core::_px] = msg->x;
    _pose.data[kraken_core::_py] = msg->y;
    _pose.data[kraken_core::_pz] = msg->depth;

    if (msg->flag)   
        _controller.local2global(_pose,_currPos);
    else
        _currPos = _pose;

    _controller.setSetPoint(_currPos);
    _controller.moveTest();
    ros::Rate looprate(10);

    kraken_msgs::advancedControllerFeedback feedback;
    kraken_msgs::advancedControllerResult result;
    while(ros::ok())
    {
        if (_server1->isPreemptRequested() || !ros::ok())
        {
            _server1->setPreempted();
             break;
        }
        feedback.running_time = _controller.checkError(_feedBack);
       // std::cout<<feedback.running_time<<std::endl;
        _server1->publishFeedback(feedback);
        if (feedback.running_time)
        {
            result.time_taken = 30;
            _server1->setSucceeded(result);
            _controller.pause();
            break;
        }

        _controller.doControlIteration(_feedBack);
        _controller.updateState();

        _set_point.publish(_currPos);
        _pub6.publish(_controller.getThruster6Value());


        ros::spinOnce();
        //std::cerr<<"looping"<<std::endl;
        looprate.sleep();
    }
    _controller.pause();
    _do_control = true;
  }
  
  void ControlServer::executeOrientationChange(const kraken_msgs::controllerGoalConstPtr &msg)
  {
    _do_control = false;
    kraken_msgs::krakenPose _pose;
    _controller.getState(_pose);
    //_pose.data[kraken_core::_pz] = msg->d;
    _pose.data[kraken_core::_roll] = msg->r;
    _pose.data[kraken_core::_pitch] = msg->p;
    _pose.data[kraken_core::_yaw] = msg->y;
    _controller.setSetPoint(_pose);
    _controller.moveTest();
    ros::Rate looprate(10);
    kraken_msgs::controllerFeedback feedback;
    kraken_msgs::controllerResult result;
    while(ros::ok())
    {
        if (_server2->isPreemptRequested() || !ros::ok())
        {
            _server2->setPreempted();
            break;
        }
        feedback.running_time = _controller.checkError(_feedBack);
        std::cout<<feedback.running_time<<std::endl;
        _server2->publishFeedback(feedback);
        if (feedback.running_time)
        {
            result.time_taken = 30;
            //_server2->setSucceeded(result);
            //_controller.pause();
            //break;
        }

        _controller.doControlIteration(_feedBack);

        if(_ip_controller)
            _controller.updateIPState();
        else
            _controller.updateState();


        _pub6.publish(_controller.getThruster6Value());
         ros::spinOnce();
         looprate.sleep();
    }
    _controller.pause();
    _do_control = true;
  }
  
  void ControlServer::loadParams(const std::vector<std::string> &filenames)
  {
    _controller.loadParams(filenames);
    _controller.pause();
  }
  
}
