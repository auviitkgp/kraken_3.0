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
#include <bits/stdc++.h>
#include "ui_main_window.h"
#include "qnode.hpp"
#include "kraken_msgs/krakenPose.h"
#include "kraken_msgs/forceData6Thruster.h"
/*****************************************************************************
** Namespace
*****************************************************************************/
Q_DECLARE_METATYPE(kraken_msgs::krakenPoseConstPtr);
Q_DECLARE_METATYPE(kraken_msgs::forceData6ThrusterConstPtr);
namespace gui_template {

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
    void state_update(const kraken_msgs::krakenPoseConstPtr &msg);
    void force_update(const kraken_msgs::forceData6ThrusterConstPtr& msg);
    void controllerButtonClicked();
    void advancedControllerButtonClicked();
    void moveAlongButtonClicked();
    void pauseButtonClicked();

Q_SIGNALS:
    void sendControllGoal(float r,float p,float y);
    void sendAdvancedControlGoal(float x,float y,float z);
    void callmoveALongsrv(float angle);
    void callPauseService();
private:
    Ui::MainWindowDesign ui;
	QNode qnode;
};

}  // namespace App

#endif // App_MAIN_WINDOW_H
