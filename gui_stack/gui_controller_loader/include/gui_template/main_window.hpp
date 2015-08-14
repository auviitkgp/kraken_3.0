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
#include <bits/stdc++.h>
#include <stdlib.h>
#include <QFile>

/*****************************************************************************
** Namespace
*****************************************************************************/

namespace gui_template
{

/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */

const std::string MOVE_FORWARD="MoveForward.cp";
const std::string MOVE_BACKWARD="MoveBack.cp";
const std::string MOVE_DEPTH="depth.cp";
const std::string STOP="Pause.cp";
const std::string DEFAULT="Test.cp";

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int argc, char** argv, QWidget *parent = 0);
    ~MainWindow();

    void ReadSettings(); // Load up qt program settings at startup
    void WriteSettings(); // Save qt program settings when closing

    void closeEvent(QCloseEvent *event); // Overloaded function

Q_SIGNALS:
    void callService(std::string name);

public Q_SLOTS:
    /******************************************
    ** Auto-connections (connectSlotsByName())
    *******************************************/
    void on_actionAbout_triggered();
    /******************************************
    ** Manual connections
    *******************************************/
    void pushButtonclicked();
    void moveForwar();
    void moveBack();
    void moveDeep();
    void stop();
private:
    QFile file;
    Ui::MainWindowDesign ui;
    QNode qnode;
    std::string pathname;

};

}  // namespace App

#endif // App_MAIN_WINDOW_H
