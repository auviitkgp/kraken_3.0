/**
 * @file /include/App/main_window.hpp
 *
 * @brief Qt based gui for App.
 *
 * @date November 2010
 **/
#ifndef App_MAIN_WINDOW_H
#define App_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui/QMainWindow>
#include "ui_main_window.h"
#include "qnode.hpp"
#include <QMetaType>
#include <qwt/qwt_dial_needle.h>
#include <qwt/qwt_compass_rose.h>

/*****************************************************************************
** Namespace
*****************************************************************************/
Q_DECLARE_METATYPE(kraken_msgs::krakenPoseConstPtr);
Q_DECLARE_METATYPE(sensor_msgs::ImageConstPtr);
Q_DECLARE_METATYPE(kraken_msgs::thrusterData4ThrusterConstPtr);
Q_DECLARE_METATYPE(kraken_msgs::thrusterData6ThrusterConstPtr);
namespace Telemetry {

/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */

class MainWindow : public QMainWindow {
Q_OBJECT

public:
	MainWindow(int argc, char** argv, QWidget *parent = 0);
	~MainWindow();

	void ReadSettings(); // Load up qt program settings at startup
	void WriteSettings(); // Save qt program settings when closing

	void closeEvent(QCloseEvent *event); // Overloaded function

public Q_SLOTS:
	/******************************************
	** Auto-connections (connectSlotsByName())
	*******************************************/
	void on_actionAbout_triggered();
        /******************************************
        ** Manual connections
        *******************************************/
        void updateCurrentPose(kraken_msgs::krakenPoseConstPtr msg);
        void updateSetPose(kraken_msgs::krakenPoseConstPtr msg);
        void updateFrontImage(sensor_msgs::ImageConstPtr _msg);
        void updateBottomImage(sensor_msgs::ImageConstPtr _msg);
private:
	void updateCurrentYaw(const float val);
	void updateSetYaw(const float val);
	void updateCurrentDepth(const float val);
	void updateSetDepth(const float val);
	void updateCurrentPitch(const float val);
	void premap();
	Ui::MainWindowDesign ui;
	QNode qnode;
};

}  // namespace App

#endif // App_MAIN_WINDOW_H
