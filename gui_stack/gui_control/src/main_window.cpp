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
#include "pose_server/KrakenPose.h"
#include "../include/gui_template/main_window.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace gui_template {

using namespace Qt;

/*****************************************************************************
** Implementation [MainWindow]
*****************************************************************************/

MainWindow::MainWindow(int argc, char** argv, QWidget *parent)
	: QMainWindow(parent)
    , qnode(argc,argv,ui)
{
    ui.setupUi(this); // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.

    setWindowIcon(QIcon(":/images/icon.png"));

    /*********************
    ** Auto Start
    **********************/
    qRegisterMetaType<kraken_msgs::krakenPoseConstPtr>("krakenPoseConstPtrT");
    qRegisterMetaType<kraken_msgs::forceData6ThrusterConstPtr>("thrusterData4ThrusterConstPtrT");


    QObject::connect(&qnode, SIGNAL(statePub(kraken_msgs::krakenPoseConstPtr)), this, SLOT(state_update(kraken_msgs::krakenPoseConstPtr)));
    QObject::connect(&qnode,SIGNAL(forcePub(kraken_msgs::forceData6ThrusterConstPtr)),this,SLOT(force_update(kraken_msgs::forceData6ThrusterConstPtr)));
    QObject::connect(this,SIGNAL(sendControllGoal(float,float,float)),&qnode,SLOT(controlGoalCB(float,float,float)));
    QObject::connect(this,SIGNAL(sendAdvancedControlGoal(float,float,float)),&qnode,SLOT(advancedGoalCB(float,float,float)));
    QObject::connect(ui.advcedcontrol_sendbtn,SIGNAL(clicked()),this,SLOT(advancedControllerButtonClicked()));
    QObject::connect(ui.controller_button,SIGNAL(clicked()),this,SLOT(controllerButtonClicked()));
    QObject::connect(ui.linesrv_movebtn,SIGNAL(clicked()),this,SLOT(moveAlongButtonClicked()));
    QObject::connect(this,SIGNAL(callmoveALongsrv(float)),&qnode,SLOT(moveAlongCB(float)));
    QObject::connect(ui.linesrv_pausebtn,SIGNAL(clicked()),this,SLOT(pauseButtonClicked()));
    QObject::connect(this,SIGNAL(callPauseService()),&qnode,SLOT(pauseCB()));
    qnode.init();
}

MainWindow::~MainWindow() {}

/*****************************************************************************
** Implementation [Slots]
*****************************************************************************/

/*****************************************************************************
** Implemenation [Slots][manually connected]
*****************************************************************************/

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

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
}

void MainWindow::force_update(const kraken_msgs::forceData6ThrusterConstPtr &msg)
{
    QString s;
    char word[50];
    sprintf(word,"%.2f %.2f %.2f %.2f %.2f %.2f",msg->data[0],msg->data[1],msg->data[2],msg->data[3],msg->data[4],msg->data[5]);
    s.append(word);
    ui.force_box->setText(s);
}

void MainWindow::controllerButtonClicked()
{
    float r,p,y;
    r=ui.roll_box->text().toFloat();
    p=ui.pitch_box->text().toFloat();
    y=ui.yaw_box->text().toFloat();
    Q_EMIT sendControllGoal(r,p,y);
    ui.textBrowser->append("Controller goal sent\n");
}

void MainWindow::advancedControllerButtonClicked()
{
    float x,y,z;
    x=ui.x_box->text().toFloat();
    y=ui.y_box->text().toFloat();
    z=ui.z_box->text().toFloat();
    Q_EMIT sendAdvancedControlGoal(x,y,z);
    ui.textBrowser->append("Advanced Controller goal sent\n");
}

void MainWindow::state_update(const kraken_msgs::krakenPoseConstPtr &msg)
{
    QString s[6];
    s[0]=QString::number(msg->data[kraken_core::_px]);
    s[1]=QString::number(msg->data[kraken_core::_py]);
    s[2]=QString::number(msg->data[kraken_core::_pz]);
    s[3]=QString::number(msg->data[kraken_core::_roll]);
    s[4]=QString::number(msg->data[kraken_core::_pitch]);
    s[5]=QString::number(msg->data[kraken_core::_yaw]);

    ui.x_val->setText(s[0]);
    ui.y_val->setText(s[1]);
    ui.z_val->setText(s[2]);

    ui.roll_val->setText(s[3]);
    ui.pitch_val->setText(s[4]);
    ui.yaw_val->setText(s[5]);
}

void MainWindow::moveAlongButtonClicked()
{
    float angle = ui.angle_box->text().toFloat();
    Q_EMIT callmoveALongsrv(angle);
}

void MainWindow::pauseButtonClicked()
{
    Q_EMIT callPauseService();
}

}  // namespace App

