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
	, qnode(argc,argv),_max_history(100),_premap_image(600,600,QImage::Format_RGB888)
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
    ** Pre map
    **********************/
    
    for(int i=0;i<600/*_premap_image.size().rheight()*/;i++)
    {
        uchar* _data = (uchar*) _premap_image.scanLine(i);
        //std::cerr<<_premap_image.bytesPerLine()<<std::endl;
        for(int j=0;j<_premap_image.bytesPerLine()/*_premap_image.size().rwidth()*/;j+=_premap_image.bytesPerLine()/600)
        {
            _data[j] = 0; 
            _data[j+1]=0;
            _data[j+2]=255;
        }
    }
    _pre_rec.y=599;
    _pre_rec.x=0;
    _pre_rec.width=10;
    _pre_rec.height=10;
    premap(0,0,0);    
    
    /*********************
    ** Plot Setting
    **********************/
    //ui.velocity_plot->axisScaleEngine(QwtPlot::xBottom)->setAttribute(QwtScaleEngine::Floating,true);
    ui.velocity_plot->axisScaleEngine(QwtPlot::yLeft)->setAttribute(QwtScaleEngine::Floating,true);
    ui.velocity_plot->setAxisScale(QwtPlot::yLeft,-1,1,0.1);
    ui.velocity_plot->setAxisScale(QwtPlot::xBottom,0,_max_history,1);
    ui.velocity_plot->enableAxis(QwtPlot::xBottom,false);
    _velocity_x_curve = new QwtPlotCurve("Velocity x Curve");
    _velocity_x_curve->setPen(QPen(Qt::green));
    _velocity_x_curve->attach(ui.velocity_plot);
    _velocity_y_curve = new QwtPlotCurve("Velocity y Curve");
    _velocity_y_curve->setPen(QPen(Qt::red));
    _velocity_y_curve->attach(ui.velocity_plot);
    _velocity_z_curve = new QwtPlotCurve("Velocity z Curve");
    _velocity_z_curve->setPen(QPen(Qt::black));
    _velocity_z_curve->attach(ui.velocity_plot);
    
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
  updateVelocityCurve(msg->data[kraken_core::_vx],msg->data[kraken_core::_vy],msg->data[kraken_core::_vz]);
  premap(msg->data[kraken_core::_px],msg->data[kraken_core::_py],msg->data[kraken_core::_pz]);
}

void MainWindow::updateSetPose(kraken_msgs::krakenPoseConstPtr msg)
{
  if(ui.tabWidget->currentIndex()==0)
  {
    updateSetDepth(msg->data[kraken_core::_pz]);
    updateSetYaw(msg->data[kraken_core::_yaw]);
  }

  QString a;
  a.arg(msg->data[kraken_core::_px]);
  ui.lineEdit->setText(a);

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

void MainWindow::premap(const float x,const float y,const float z)
{
  // Reset Previsous map
  for(int i=_pre_rec.y;i>=0&&i>(_pre_rec.y-5);i--)
  {
      uchar* _data = (uchar*) _premap_image.scanLine(i);
      for(int j=_pre_rec.x*3;j<(_pre_rec.x+_pre_rec.width)*3;j+=3)
      {
          _data[j]=0;
          _data[j+1]=0;
          _data[j+2]=255;
      }
  }
  // Add path
  for(int i=_pre_rec.y;i>=0&&i>(_pre_rec.y-2);i--)
  {
      uchar* _data = (uchar*) _premap_image.scanLine(i);
      for(int j=_pre_rec.x*3;j<(_pre_rec.x+2)*3;j+=3)
      {
          _data[j]=255;
          _data[j+1]=0;
          _data[j+2]=0;
      }
  }
  if(x>0&&(int)(x*20)<600)
  {
    _pre_rec.x = (int)(x*20);
  }
  else
  {
    _pre_rec.x=0;  
  }
  if(y>0&&(int)(y*20)<600)
  {
    _pre_rec.y = 599-(int)(y*20);
  }
  else
  {
    _pre_rec.y=0;  
  }
  // Add Vehicle
  for(int i=_pre_rec.y;i>=0&&i>(_pre_rec.y-5);i--)
  {
      uchar* _data = (uchar*) _premap_image.scanLine(i);
      for(int j=_pre_rec.x*3;j<(_pre_rec.x+5)*3;j+=3)
      {
          _data[j]=255;
          _data[j+1]=255;
          _data[j+2]=0;
      }
  }
  if(ui.tabWidget->currentIndex()==0)
  {
    ui.pre_map_label->setPixmap(QPixmap::fromImage(_premap_image));
  }
}

void MainWindow::updateCurrentDepth(const float val)
{
  ui.curr_depth_thermo->setValue(5.0-val);
}

void MainWindow::updateSetDepth(const float val)
{
  ui.set_depth_thermo->setValue(5.0-val);
}

void MainWindow::updateVelocityCurve(const float vx, const float vy, const float vz)
{
  _velocity_x_vec.push_back(QPointF(_count_velocity,vx));
  _velocity_y_vec.push_back(QPointF(_count_velocity,vy));
  _velocity_z_vec.push_back(QPointF(_count_velocity,vz));
  if(_count_velocity<_max_history)
  {
    
  }
  else
  {
    _velocity_x_vec.pop_front();
    _velocity_y_vec.pop_front();
    _velocity_z_vec.pop_front();
    if(ui.tabWidget->currentIndex()==0)
    {
      for(int i=0;i<_velocity_x_vec.size();i++)
      {
          _velocity_x_vec[i].setX(i);
          _velocity_y_vec[i].setX(i);
          _velocity_z_vec[i].setX(i);
      }
      _velocity_x_data.setSamples(_velocity_x_vec);
      _velocity_y_data.setSamples(_velocity_y_vec);
      _velocity_z_data.setSamples(_velocity_z_vec);
      _velocity_x_curve->setData(&_velocity_x_data);
      _velocity_y_curve->setData(&_velocity_y_data);
      _velocity_z_curve->setData(&_velocity_z_data);
      ui.velocity_plot->replot();
    }
  }
  _count_velocity++;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  QMainWindow::closeEvent(event);
}

}  // namespace App

