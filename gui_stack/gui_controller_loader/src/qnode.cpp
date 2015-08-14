/**
 * @file /src/qnode.cpp
 *
 * @brief Ros communication central!
 *
 * @date February 2011
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <ros/network.h>
#include <string>
#include <std_msgs/String.h>
#include <sstream>
#include "../include/gui_template/qnode.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace gui_template
{

/*****************************************************************************
** Implementation
*****************************************************************************/


QNode::QNode(int argc, char** argv,std::string* ptr ) :
    init_argc(argc),
    init_argv(argv),
    pathname_ptr(ptr)

{}

QNode::~QNode()
{
    if(ros::isStarted())
    {
        ros::shutdown(); // explicitly needed since we use ros::start();
        ros::waitForShutdown();
    }

    wait();
}

bool QNode::init()
{
    ros::init(init_argc,init_argv,"App");
    ros::start(); // explicitly needed since our nodehandle is going out of scope.
    ros::NodeHandle n;
    // Add your ros communications here.


    chatter_publisher = n.advertise<std_msgs::String>("chatter", 1000);
    client=n.serviceClient<control_server::loadParam>(topics::CONTROL_LOADPARAM);

    dyn_server=new dynamic_reconfigure::Server<gui_controller_loader::paramsConfig>();
    dyn_servCB_global=new dynamic_reconfigure::Server<gui_controller_loader::paramsConfig>::CallbackType();


    *dyn_servCB_global=boost::bind(&QNode::callbackCB,this,_1,_2);
    dyn_server->setCallback(*dyn_servCB_global);

    start();
    return true;
}

void QNode::loadParamCB(std::string name)
{
    control_server::loadParam srv;
    srv.request.file=name;
    client.call(srv);
}




void QNode::callbackCB(gui_controller_loader::paramsConfig &config, uint32_t level)
{
    ROS_INFO("call back called");
    cp_obj.load((*pathname_ptr+PARAM_SAVER).c_str());
    float **gain_matrix=cp_obj.getGain();
    float *offset_arr=cp_obj.getOffset();

    ROS_INFO("before");

    for (int i = 0; i < cp_obj.getRows(); ++i)
    {
        for (int j = 0; j < cp_obj.getColumns(); ++j)
        {
            printf("%f\t",gain_matrix[i][j]);
        }

        printf("\n");
    }

    offset_arr[OFF_YAW.first]=config.off_yaw;
    gain_matrix[KP_YAW.first][KP_YAW.second]=config.Kp_yaw;
    gain_matrix[KD_YAW.first][KD_YAW.second]=config.Kd_yaw;
    gain_matrix[KI_YAW.first][KI_YAW.second]=config.Ki_yaw;
    gain_matrix[KP_YAW_NEG.first][KP_YAW_NEG.second]=-config.Kp_yaw;
    gain_matrix[KD_YAW_NEG.first][KD_YAW_NEG.second]=-config.Kd_yaw;
    gain_matrix[KI_YAW_NEG.first][KI_YAW_NEG.second]=-config.Ki_yaw;

    //bottom----- pitch is positive=;
    offset_arr[off_dep_bot.first]=config.off_dep_bot;
    gain_matrix[Kib_depth.first][Kib_depth.second]=config.Kib_depth;
    gain_matrix[Kpb_depth.first][Kpb_depth.second]=config.Kpb_depth;
    gain_matrix[Kdb_depth.first][Kdb_depth.second]=config.Kdb_depth;



    offset_arr[off_dep_top.first]=config.off_dep_top;
    gain_matrix[Kit_depth.first][Kit_depth.second]=config.Kit_depth;
    gain_matrix[Kpt_depth.first][Kpt_depth.second]=config.Kpt_depth;
    gain_matrix[Kdt_depth.first][Kdt_depth.second]=config.Kdt_depth;

    ROS_INFO("after");

    for (int i = 0; i < cp_obj.getRows(); ++i)
    {
        for (int j = 0; j < cp_obj.getColumns(); ++j)
        {
            printf("%f\t",gain_matrix[i][j]);
        }

        printf("\n");
    }



    FILE* outfile=fopen((*pathname_ptr+PARAM_SAVER).c_str(),"w");
    cp_obj.write(outfile);
    fflush(outfile);
    fclose(outfile);
    loadParamCB((*pathname_ptr+PARAM_SAVER));
}

void QNode::run()
{
    ros::Rate loop_rate(10);
    int count = 0;

    while ( ros::ok() )
    {
        loop_rate.sleep();
    }

    ros::spin();

    std::cout << "Ros shutdown, proceeding to close the gui." << std::endl;
    Q_EMIT rosShutdown(); // used to signal the gui for a shutdown (useful to roslaunch)
}


}  // namespace App
