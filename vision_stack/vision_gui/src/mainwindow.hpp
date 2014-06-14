#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <ui_mainwindow.h>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QTimer>
#include <QImage>
#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <opencv2/core/mat.hpp>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <QtCore>
class MyLabel : public QLabel
{
    
    Q_OBJECT
public:
Q_SIGNALS:
    //
    void on_mouseDoubleClickEvent(QMouseEvent * ev);
    
protected:
    void mouseDoubleClickEvent(QMouseEvent * ev)
    {
        Q_EMIT on_mouseDoubleClickEvent(ev);
    }
};

class MyWindow : public QMainWindow
{
    Q_OBJECT

public:
    MyWindow(QWidget *parent = 0);
    ~MyWindow();
    void closeEvent(QCloseEvent *);
public Q_SLOTS:
    // button callback
    void on_openFile_clicked();
    void on_closeFile_clicked();
    void on_writeFile_clicked();
    void on_newFile_clicked();

    void on_openCamera_clicked();
    void on_startCamera_clicked();
    void on_pauseCamera_clicked();
    void on_closeCamera_clicked();
    // slider callbacks
    void on_HValue_slider_valueChanged(int value);
    void on_SValue_slider_valueChanged(int value);
    void on_VValue_slider_valueChanged(int value);
    void on_HThreshValue_slider_valueChanged(int value);
    void on_SThreshValue_slider_valueChanged(int value);
    void on_VThreshValue_slider_valueChanged(int value);
    void on_colorImage_mouseDoubleClickEvent(QMouseEvent * ev);

    /******** Filters ****************/

    void on_blurSlider_valueChanged(int value);
    void on_mblurSlider_valueChanged(int value);
    void on_gblurSlider_valueChanged(int value);

    void on_dilateCheck_clicked();
    void on_erodeCheck_clicked();

    void on_morphellipse_clicked();
    void on_morphrect_clicked();
    void on_morphcross_clicked();

    void on_kernelSizeSlider_valueChanged(int value);

    void on_contoursCheck_clicked();

    void on_boundrectCheck_clicked();
    void on_boundcircleCheck_clicked();
    void on_approxpolyCheck_clicked();

    void on_houghCheck_clicked();

    void on_houghMinRadius_valueChanged(int value);
    void on_houghMaxRadius_valueChanged(int value);

    void on_houghLinesCheck_clicked();

    void on_blobCheck_clicked();

    void on_backgroundRSlider_valueChanged(int value);
    //     void on_backgroundGSlider_valueChanged(int value);
    //     void on_backgroundBSlider_valueChanged(int value);

    void on_foregroundRSlider_valueChanged(int value);
    //     void on_foregroundGSlider_valueChanged(int value);
    //     void on_foregroundBSlider_valueChanged(int value);

    void on_areablobSlider_valueChanged(int value);
    void on_areablobLess_clicked();
    void on_areablobMore_clicked();
    void on_blobInclude_clicked();
    void on_blobExclude_clicked();

    // manual callbacks
    void on_updateImages();
    //i->colorImage->mouseDoubleClickEvent();
private:
    Ui::MainWindow *ui;
    bool camera_opend,video_being_processed;
    cv::Scalar start,end;
    std::string fileName;
    QTimer *timer;
    cv::Mat colorImage,displayImage,thImage,binaryImage,camImage;
    cv::VideoCapture cam;
    cv::Size image_size;
    MyLabel *colorLabel;
    cv::Vec3b vec;
    bool getNextFrame(cv::Mat &img);
    void displayImages();
    void setThresh();

    bool source_from_log;
//    QString fn_video; // the file name of the log video.

    int mblurVal, gblurVal, blurVal;

    bool dilate_b, erode_b;

    int kernelSizeVal, kernelShape;

    bool draw_contours;

    bool rectContours, circleContours, polyContours;

    bool hough_enabled, hough_lines_enabled;

    int min_radius, max_radius;

    bool blob_enabled;

    //    int bg_r, bg_g, bg_b, fg_r, fg_g, fg_b;

    int bg_r, fg_r;

    int filter_arg2, filter_arg4, filter_arg5_area;
};

#endif // MAINWINDOW_H
