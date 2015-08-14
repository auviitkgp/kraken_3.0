/**
 * @file /include/App/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef App_QNODE_HPP_
#define App_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/
#include "resources/topicHeader.h"
#include <ros/ros.h>
#include <string>
#include <QThread>
#include <QStringListModel>
#include "control_server/loadParam.h"

#include "gui_controller_loader/paramsConfig.h"
#include "dynamic_reconfigure/server.h"
#include "controller_basic/ControlParameters.h"


/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace gui_template
{

/*****************************************************************************
** Class
*****************************************************************************/
const std::string PARAM_SAVER="paramSaver.cp";
const std::pair<int,int>

OFF_YAW(4,0),
        KP_YAW(4,17),
        KD_YAW(4,20),
        KI_YAW(4,14),
        KP_YAW_NEG(5,17),
        KD_YAW_NEG(5,20),
        KI_YAW_NEG(5,14),


        //bottom----- pitch is positive
        off_dep_bot(0,0),
        Kib_depth(0,13),
        Kpb_depth(0,16),
        Kdb_depth(0,19),



        off_dep_top(1,0),
        Kit_depth(1,13),
        Kpt_depth(1,16),
        Kdt_depth(1,19)
        ;
class QNode : public QThread
{
    Q_OBJECT
public:
    QNode(int argc, char** argv,std::string* );

    virtual ~QNode();
    bool init();
    void run();
public Q_SLOTS:
    void loadParamCB(std::string name);

    void callbackCB(gui_controller_loader::paramsConfig &config, uint32_t level);

Q_SIGNALS:
    void rosShutdown();

private:
    int init_argc;
    char** init_argv;
    ros::Publisher chatter_publisher;
    ros::ServiceClient client;

    dynamic_reconfigure::Server<gui_controller_loader::paramsConfig> *dyn_server;
    kraken_controller::ControlParameters cp_obj;
    std::string* pathname_ptr;
    dynamic_reconfigure::Server<gui_controller_loader::paramsConfig>::CallbackType *dyn_servCB_global;

};

}  // namespace App

#endif /* App_QNODE_HPP_ */
