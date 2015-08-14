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
#include "../include/gui_template/main_window.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace gui_template
{

using namespace Qt;

/*****************************************************************************
** Implementation [MainWindow]
*****************************************************************************/
using namespace std;
MainWindow::MainWindow(int argc, char** argv, QWidget *parent)
    : QMainWindow(parent)

    , qnode(argc,argv,&pathname)

{
    ui.setupUi(this); // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.
    char params[1000];

    pathname=argv[1];
    file.open(fopen((pathname+DEFAULT).c_str(),"r+"),QIODevice::ReadWrite);
    QString text;

    QTextStream in(fopen((pathname+DEFAULT).c_str(),"r+"));
    text=in.readAll();
    ui.textEdit->setText(text);
    setWindowIcon(QIcon(":/images/icon.png"));

    QObject::connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(pushButtonclicked()));
    QObject::connect(this,SIGNAL(callService(std::string)),&qnode,SLOT(loadParamCB(std::string)));


    QObject::connect(ui.forwardButton,SIGNAL(clicked()),this,SLOT(moveForwar()));
    QObject::connect(ui.backButton,SIGNAL(clicked()),this,SLOT(moveBack()));
    QObject::connect(ui.depthButton,SIGNAL(clicked()),this,SLOT(moveDeep()));
    QObject::connect(ui.stopButton,SIGNAL(clicked()),this,SLOT(stop()));

    /*********************
    ** Auto Start
    **********************/
    qnode.init();
}

MainWindow::~MainWindow()
{
    file.close();
}

/*****************************************************************************
** Implementation [Slots]
*****************************************************************************/

/*****************************************************************************
** Implemenation [Slots][manually connected]
*****************************************************************************/


/*****************************************************************************
** Implementation [Menu]
*****************************************************************************/

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About ..."),tr("<h2>PACKAGE_NAME Test Program 0.10</h2><p>Copyright Yujin Robot</p><p>This package needs an about description.</p>"));
}

/*****************************************************************************
** Implementation [Configuration]
*****************************************************************************/

void MainWindow::ReadSettings()
{
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

void MainWindow::WriteSettings()
{
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

void MainWindow::pushButtonclicked()
{
//    fprintf(fp,"%s",ui.textEdit->toPlainText().toStdString().c_str());
    file.write(ui.textEdit->toPlainText().toStdString().c_str());
    file.close();
    Q_EMIT(callService((pathname+DEFAULT).c_str()));
    file.open(fopen((pathname+DEFAULT).c_str(),"r+"),QIODevice::ReadWrite);
}

void MainWindow::moveForwar()
{
    Q_EMIT(callService((pathname+MOVE_FORWARD).c_str()));
}

void MainWindow::moveBack()
{
    Q_EMIT(callService((pathname+MOVE_BACKWARD).c_str()));
}

void MainWindow::moveDeep()
{
    Q_EMIT(callService((pathname+MOVE_DEPTH).c_str()));
}

void MainWindow::stop()
{
    Q_EMIT(callService((pathname+STOP).c_str()));
}

}  // namespace App


