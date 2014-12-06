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
#include <bits/stdc++.h>
#include <QtGui/QMainWindow>
#include "ui_main_window.h"
#include "qnode.hpp"
#include <QMetaType>
#include <qwt/qwt_dial_needle.h>
#include <qwt/qwt_compass_rose.h>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_scale_engine.h>
#include <opencv/cv.h>

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
	void updateVelocityCurve(const float vx,const float vy,const float vz);
	void premap(const float x,const float y,const float z);
	Ui::MainWindowDesign ui;
	QNode qnode;
	int _count_velocity;
	const int  _max_history;
	// Velocity x
	QwtPlotCurve* _velocity_x_curve;
	QwtPointSeriesData _velocity_x_data;
	QVector<QPointF> _velocity_x_vec;
	// Velocity y
	QwtPlotCurve* _velocity_y_curve;
	QwtPointSeriesData _velocity_y_data;
	QVector<QPointF> _velocity_y_vec;
	// Velocity z
	QwtPlotCurve* _velocity_z_curve;
	QwtPointSeriesData _velocity_z_data;
	QVector<QPointF> _velocity_z_vec;
	// Pre map
	QImage _premap_image;
	cv::Rect _pre_rec;
};

}  // namespace App

#endif // App_MAIN_WINDOW_H
