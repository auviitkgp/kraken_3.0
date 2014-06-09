#include <ros/ros.h>
#include <control_server/ControlServer.h>
#include <control_server/AuvController.h>
#include <kraken_msgs/switchControllers.h>
#include <kraken_msgs/moveAlongLine.h>
#include <controller_basic/ControlParameters.h>
#include <controller_basic/StateController.h>
#include <math.h>

int main(int argc,char** argv)
{

    ros::init(argc, argv, "control_server_client");

    ros::NodeHandle n;
    if(argc>1)
    {
        ros::ServiceClient client = n.serviceClient<kraken_msgs::moveAlongLine>("/kraken/translate_auv");
        kraken_msgs::moveAlongLine srv;


        srv.request.type = atoi(argv[1]);

        if(atoi(argv[1]) <= -2)
        {
            srv.request.angle = 180;
        }
        else
        {
            srv.request.angle = 0;
        }
        std::cerr<<srv.request.angle<<"\t"<<atoi(argv[1])<<std::endl;
        if (client.call(srv))
        {
          ROS_INFO("Sum: %d", srv.response.result);
        }
        else
        {
          ROS_ERROR("ERROR");
          return 1;
        }
    }
    else
    {
        std::cerr<<"Give motion command 3.0 for forward -3.0 for backward 0.0 for pause "<<std::endl;
    }

  return 0;
}
