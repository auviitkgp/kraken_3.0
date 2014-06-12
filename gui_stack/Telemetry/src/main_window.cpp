/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date February 2011
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include "../include/Telemetry/main_window.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace Telemetry {

using namespace Qt;

/*****************************************************************************
** Implementation [MainWindow]
*****************************************************************************/

MainWindow::MainWindow(int argc, char** argv, QWidget *parent)
	: QMainWindow(parent)
	, qnode(argc,argv)
{
    ui.setupUi(this); // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.

    setWindowIcon(QIcon(":/images/icon.png"));
    
    ui.curr_yaw_compass->setNeedle(new QwtCompassMagnetNeedle(QwtCompassMagnetNeedle::TriangleStyle,Qt::white,Qt::green));
    //ui.curr_yaw_compass->setScale(360,0,0.1);
    ui.set_yaw_compass->setNeedle(new QwtCompassMagnetNeedle(QwtCompassMagnetNeedle::TriangleStyle,Qt::white,Qt::red));
    ui.pitch_compass->setNeedle(new QwtCompassMagnetNeedle(QwtCompassMagnetNeedle::TriangleStyle,Qt::white,Qt::gray));
    /*********************
    ** Mannual Connection
    **********************/
    qRegisterMetaType<sensor_msgs::ImageConstPtr>("ImageConstPtr");
    qRegisterMetaType<kraken_msgs::krakenPoseConstPtr>("krakenPoseConstPtr");
    qRegisterMetaType<kraken_msgs::thrusterData4ThrusterConstPtr>("thrusterData4ThrusterConstPtr");
    qRegisterMetaType<kraken_msgs::thrusterData6ThrusterConstPtr>("thrusterData6ThrusterConstPtr");
    QObject::connect(&qnode, SIGNAL(updateCurrentPose(kraken_msgs::krakenPoseConstPtr)), this, SLOT(updateCurrentPose(kraken_msgs::krakenPoseConstPtr)));
    QObject::connect(&qnode, SIGNAL(updateSetPose(kraken_msgs::krakenPoseConstPtr)), this, SLOT(updateSetPose(kraken_msgs::krakenPoseConstPtr)));
    QObject::connect(&qnode, SIGNAL(updateFrontImage(sensor_msgs::ImageConstPtr)), this, SLOT(updateFrontImage(sensor_msgs::ImageConstPtr)));
    QObject::connect(&qnode, SIGNAL(updateBottomImage(sensor_msgs::ImageConstPtr)), this, SLOT(updateBottomImage(sensor_msgs::ImageConstPtr)));
    /*********************
    ** Auto Start
    **********************/
    updateCurrentDepth(0);
    updateSetDepth(0);
    updateCurrentYaw(0);
    updateSetYaw(0);
    updateCurrentPitch(0);
    qnode.init();
}

MainWindow::~MainWindow() {}

/*****************************************************************************
** Implementation [Slots]
*****************************************************************************/

/*****************************************************************************
** Implemenation [Slots][manually connected]
*****************************************************************************/
void MainWindow::updateFrontImage(sensor_msgs::ImageConstPtr _msg)
{
  if(ui.tabWidget->currentIndex()==1)
  {
    cv::Mat msg;  
    cv_bridge::CvImagePtr _imagePtr;
    try
    {
        _imagePtr = cv_bridge::toCvCopy(_msg, "bgr8");
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", _msg->encoding.c_str());
    }
    msg = _imagePtr->image;
    cv::cvtColor(msg,msg,CV_BGR2RGB);
    cv::resize(msg,msg,cv::Size(480,320));
    QImage display1((uchar *)msg.data,msg.cols,
                    msg.rows,QImage::Format_RGB888);
    ui.front_cam_label->setPixmap(QPixmap::fromImage(display1));
  }
}

void MainWindow::updateBottomImage(sensor_msgs::ImageConstPtr _msg)
{
  if(ui.tabWidget->currentIndex()==1)
  {
    cv::Mat msg;  
    cv_bridge::CvImagePtr _imagePtr;
    try
    {
        _imagePtr = cv_bridge::toCvCopy(_msg, "bgr8");
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", _msg->encoding.c_str());
    }
    msg = _imagePtr->image;  
    cv::cvtColor(msg,msg,CV_BGR2RGB);
    cv::resize(msg,msg,cv::Size(480,320));
    QImage display1((uchar *)msg.data,msg.cols,
                    msg.rows,QImage::Format_RGB888);
    ui.bottom_cam_label->setPixmap(QPixmap::fromImage(display1));
  }
}
void MainWindow::updateCurrentPose(kraken_msgs::krakenPoseConstPtr msg)
{
  if(ui.tabWidget->currentIndex()==0)
  {
    updateCurrentDepth(msg->data[kraken_core::_pz]);
    updateCurrentYaw(msg->data[kraken_core::_yaw]);
    updateCurrentPitch(msg->data[kraken_core::_pitch]);
  }
}

void MainWindow::updateSetPose(kraken_msgs::krakenPoseConstPtr msg)
{
  if(ui.tabWidget->currentIndex()==0)
  {
    updateSetDepth(msg->data[kraken_core::_pz]);
    updateSetYaw(msg->data[kraken_core::_yaw]);
  }
}

/*****************************************************************************
** Implementation [Menu]
*****************************************************************************/

void MainWindow::on_actionAbout_triggered() {
    QMessageBox::about(this, tr("About ..."),tr("<h2>PACKAGE_NAME Test Program 0.10</h2><p>Copyright Yujin Robot</p><p>This package needs an about description.</p>"));
}

/*****************************************************************************
** Implementation [Configuration]
*****************************************************************************/

void MainWindow::ReadSettings() {
    /*QSettings settings("Qt-Ros Package", "App");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    QString master_url = settings.value("master_url",QString("http://192.168.1.2:11311/")).toString();
    QString host_url = settings.value("host_url", QString("192.168.1.3")).toString();
    //QString topic_name = settings.value("topic_name", QString("/chatter")).toString();
    ui.line_edit_master->setText(master_url);
    ui.line_edit_host->setText(host_url);
    //ui.line_edit_topic->setText(topic_name);
    bool remember = settings.value("remember_settings", false).toBool();
    ui.checkbox_remember_settings->setChecked(remember);
    bool checked = settings.value("use_environment_variables", false).toBool();
    ui.checkbox_use_environment->setChecked(checked);
    if ( checked ) {
    	ui.line_edit_master->setEnabled(false);
    	ui.line_edit_host->setEnabled(false);
    	//ui.line_edit_topic->setEnabled(false);
    }*/
}

void MainWindow::WriteSettings() {
    /*QSettings settings("Qt-Ros Package", "App");
    settings.setValue("master_url",ui.line_edit_master->text());
    settings.setValue("host_url",ui.line_edit_host->text());
    //settings.setValue("topic_name",ui.line_edit_topic->text());
    settings.setValue("use_environment_variables",QVariant(ui.checkbox_use_environment->isChecked()));
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("remember_settings",QVariant(ui.checkbox_remember_settings->isChecked()));*/

}
/*****************************************************************************
** Implementation [Functions]
*****************************************************************************/
void MainWindow::updateCurrentYaw(const float val)
{
  ui.curr_yaw_compass->setValue((int)(val/(2*pi)*360));
}

void MainWindow::updateSetYaw(const float val)
{
  ui.set_yaw_compass->setValue((int)(val/(2*pi)*360));
}

void MainWindow::updateCurrentPitch(const float val)
{
  ui.pitch_compass->setValue((int)(val/(2*pi)*360));
}

void MainWindow::premap()
{
  
}

void MainWindow::updateCurrentDepth(const float val)
{
  ui.curr_depth_thermo->setValue(5.0-val);
}

void MainWindow::updateSetDepth(const float val)
{
  ui.set_depth_thermo->setValue(5.0-val);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  QMainWindow::closeEvent(event);
}

}  // namespace App

