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

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace gui_template {

/*****************************************************************************
** Class
*****************************************************************************/

class QNode : public QThread {
    Q_OBJECT
public:
	QNode(int argc, char** argv );
	virtual ~QNode();
	bool init();
	void run();
public Q_SLOTS:
    void loadParamCB(std::string name);
Q_SIGNALS:
    void rosShutdown();

private:
	int init_argc;
	char** init_argv;
	ros::Publisher chatter_publisher;
    ros::ServiceClient client;
};

}  // namespace App

#endif /* App_QNODE_HPP_ */
