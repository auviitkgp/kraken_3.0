#include "mainwindow.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include <blob/blob.h>
#include <blob/BlobResult.h>
#include <blob/BlobContour.h>
#include <blob/BlobLibraryConfiguration.h>
#include <blob/BlobOperators.h>
#include <blob/ComponentLabeling.h>
#include <blob/BlobProperties.h>

QString fn_video;

MyWindow::MyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int time = 100;
    colorLabel = new MyLabel();
    colorLabel->setMinimumSize(QSize(320, 240));
    colorLabel->setMaximumSize(QSize(320, 240));
    colorLabel->setAlignment(Qt::AlignCenter);
    ui->verticalLayout->addWidget(colorLabel);
    ui->openCamera->setEnabled(true);
    ui->startCamera->setEnabled(false);
    ui->pauseCamera->setEnabled(false);
    ui->closeCamera->setEnabled(false);
    ui->openFile->setEnabled(true);
    ui->writeFile->setEnabled(true);
    ui->closeFile->setEnabled(false);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(on_updateImages()));
    connect(colorLabel,SIGNAL(on_mouseDoubleClickEvent(QMouseEvent* )),this,SLOT(on_colorImage_mouseDoubleClickEvent(QMouseEvent*)));
    timer->start(time);
    video_being_processed = false;
    camera_opend = false;

    source_from_log = false;

    mblurVal = 1;
    gblurVal = 1;
    blurVal = 1;

    dilate_b = false;
    erode_b = false;

    kernelSizeVal = 3;
    kernelShape = 0;

    draw_contours = false;

    ui->boundrectCheck->setEnabled(draw_contours);
    ui->boundcircleCheck->setEnabled(draw_contours);
    ui->approxpolyCheck->setEnabled(draw_contours);

    rectContours = false;
    circleContours = false;
    polyContours = false;

    min_radius = 0;
    max_radius = 0;

    hough_enabled = false;
    hough_lines_enabled = false;

    blob_enabled = false;

    bg_r = 255;
    fg_r = 255;
    filter_arg2 = B_LESS;
    filter_arg4 = B_INCLUDE;
    filter_arg5_area = 500;
}

void MyWindow::setThresh()
{
    int H_max = ui->HValue_slider->value()+ui->HThreshValue_slider->value();
    int S_max = ui->SValue_slider->value()+ui->SThreshValue_slider->value();
    int V_max = ui->VValue_slider->value()+ui->VThreshValue_slider->value();
    int H_min = ui->HValue_slider->value()-ui->HThreshValue_slider->value();
    int S_min = ui->SValue_slider->value()-ui->SThreshValue_slider->value();
    int V_min = ui->VValue_slider->value()-ui->VThreshValue_slider->value();
    if(H_max>255)
    {
        H_max=255;
    }
    if(H_min<0)
    {
        H_min=0;
    }
    if(S_max>255)
    {
        S_max=255;
    }
    if(S_min<0)
    {
        S_min=0;
    }
    if(V_max>255)
    {
        V_max=255;
    }
    if(V_min<0)
    {
        V_min=0;
    }
    start = cv::Scalar(H_min,S_min,V_min);
    end = cv::Scalar(H_max,S_max,V_max);
}

void MyWindow::on_openFile_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    "/home/","*.th");
    if(!filename.isEmpty())
    {
        fileName = filename.toStdString();
        std::cout<<fileName<<std::endl;
        std::ifstream file(fileName.c_str());
        if(file.is_open())
        {
            file.close();
            ui->openFile->setEnabled(false);
            ui->closeFile->setEnabled(true);
        }
        else
        {
            QString name(fileName.c_str());
            QMessageBox::question(  this,
                                    tr("Error"),
                                    tr("Cannot open file \"%1\"?").arg(name),
                                    QMessageBox::Ok);
        }
    }
}

void MyWindow::on_writeFile_clicked()
{
    std::ofstream file(fileName.c_str());
    if(file.is_open())
    {
        int H_max = ui->HValue_slider->value()+ui->HThreshValue_slider->value();
        int S_max = ui->SValue_slider->value()+ui->SThreshValue_slider->value();
        int V_max = ui->VValue_slider->value()+ui->VThreshValue_slider->value();
        int H_min = ui->HValue_slider->value()-ui->HThreshValue_slider->value();
        int S_min = ui->SValue_slider->value()-ui->SThreshValue_slider->value();
        int V_min = ui->VValue_slider->value()-ui->VThreshValue_slider->value();
        if(H_max>255)
        {
            H_max=255;
        }
        if(H_min<0)
        {
            H_min=0;
        }
        if(S_max>255)
        {
            S_max=255;
        }
        if(S_min<0)
        {
            S_min=0;
        }
        if(V_max>255)
        {
            V_max=255;
        }
        if(V_min<0)
        {
            V_min=0;
        }
        file<<H_min<<std::endl;
        file<<S_min<<std::endl;
        file<<V_min<<std::endl;
        file<<H_max<<std::endl;
        file<<S_max<<std::endl;
        file<<V_max<<std::endl;
        file.close();
    }
    else
    {
        QString name(fileName.c_str());
        QMessageBox::question(  this,
                                tr("Error"),
                                tr("Cannot open file \"%1\"?").arg(name),
                                QMessageBox::Ok);
        ui->openFile->setEnabled(true);
        ui->writeFile->setEnabled(false);
        ui->closeFile->setEnabled(false);
    }
}

void MyWindow::on_closeFile_clicked()
{
    fileName.clear();
    ui->openFile->setEnabled(true);
    ui->writeFile->setEnabled(false);
    ui->closeFile->setEnabled(false);
}

void MyWindow::on_newFile_clicked(){

    std::ofstream fout;
    std::ofstream fout2;

    fout.open("threshold.th");
    fout2.open("params.txt");

    if(fout.is_open())
    {
        int H_max = ui->HValue_slider->value()+ui->HThreshValue_slider->value();
        int S_max = ui->SValue_slider->value()+ui->SThreshValue_slider->value();
        int V_max = ui->VValue_slider->value()+ui->VThreshValue_slider->value();
        int H_min = ui->HValue_slider->value()-ui->HThreshValue_slider->value();
        int S_min = ui->SValue_slider->value()-ui->SThreshValue_slider->value();
        int V_min = ui->VValue_slider->value()-ui->VThreshValue_slider->value();
        if(H_max>255)
        {
            H_max=255;
        }
        if(H_min<0)
        {
            H_min=0;
        }
        if(S_max>255)
        {
            S_max=255;
        }
        if(S_min<0)
        {
            S_min=0;
        }
        if(V_max>255)
        {
            V_max=255;
        }
        if(V_min<0)
        {
            V_min=0;
        }
        fout<<H_min<<std::endl;
        fout<<S_min<<std::endl;
        fout<<V_min<<std::endl;
        fout<<H_max<<std::endl;
        fout<<S_max<<std::endl;
        fout<<V_max<<std::endl;
        fout.close();
    }
    else
    {
        QMessageBox::question(  this,
                                tr("Error"),
                                "Cannot open a new file. Disk full?",
                                QMessageBox::Ok);
        ui->openFile->setEnabled(true);
        ui->writeFile->setEnabled(false);
        ui->closeFile->setEnabled(false);
        return;
    }

    if(fout2.is_open())
    {
        using namespace std;

        fout2 << "Simple filters" << endl;
        fout2 << "=========" << endl;
        fout2 << "Simple blur : " << blurVal << endl;
        fout2 << "Median blur : " << mblurVal << endl;
        fout2 << "Gaussian blur : " << gblurVal << endl << endl;

        fout2 << "Dilate and Erode" << endl;
        fout2 << "=========" << endl;

        if(dilate_b)

            fout2 << "Dilate with ";

        if(erode_b)

            fout2 << "Erode with ";

        if(dilate_b || erode_b)
        {
            if(kernelShape == 0)

                fout2 << "MORPH_ELLIPSE";

            if(kernelShape == 1)

                fout2 << "MORPH_RECT";

            if(kernelShape == 2)

                fout2 << "MORPH_CROSS";

        }

        else

            fout2 << "Dilate and erode not applied.";

        fout2 << endl << endl;

        fout2 << "Blob Detection" << endl;
        fout2 << "=========" << endl;
        fout2 << "Background color : " << bg_r << endl;
        fout2 << "Foreground color : " << fg_r << endl;
        fout2 << "Area limit : " << filter_arg5_area;
        fout2 << endl << endl;

        fout2 << "Blob Criteria : ";

        if(filter_arg2 == B_INCLUDE)

            fout2 << "INCLUDE " ;

        else

            fout2 << "EXCLUDE";

        fout2 << " all with Area ";

        if(filter_arg4 == B_LESS)

            fout2 << " Less ";

        else

            fout2 << " Greater ";

        fout2<< " than " << filter_arg5_area << endl;

    }

    else
    {
        QMessageBox::question(  this,
                                tr("Error"),
                                "Cannot open a new file. Disk full?",
                                QMessageBox::Ok);
        ui->openFile->setEnabled(true);
        ui->writeFile->setEnabled(false);
        ui->closeFile->setEnabled(false);
        return;
    }

    QMessageBox::question(  this,
                            tr("Success"),
                            "Wrote the parameters to two files, threshold.th and params.txt.",
                            QMessageBox::Ok);

}

void MyWindow::on_openCamera_clicked()
{
    video_being_processed = false;
    camera_opend = false;
    std::cout<<ui->cameraNo->currentIndex()<<std::endl;

    if (ui->cameraNo->currentIndex() == 2){
        fn_video = QFileDialog::getOpenFileName(this,
                                                tr("Open File"),
                                                "/home/","*.avi");

        std::cout <<"Opening : " << fn_video.toUtf8().constData() << std::endl;

        if(cam.open(fn_video.toUtf8().constData())){
            camera_opend = true;
            source_from_log = true;
            ui->openCamera->setEnabled(false);
            ui->startCamera->setEnabled(true);
            ui->pauseCamera->setEnabled(false);
            ui->closeCamera->setEnabled(false);

        }

        else{

            QMessageBox::question(  this,
                                    tr("Error"),
                                    "Could not open the requested video.",
                                    QMessageBox::Ok);


        }
    }

    else

        if(cam.open(ui->cameraNo->currentIndex()))
        {
            camera_opend = true;
            ui->openCamera->setEnabled(false);
            ui->startCamera->setEnabled(true);
            ui->pauseCamera->setEnabled(false);
            ui->closeCamera->setEnabled(false);
        }
        else
        {

            QMessageBox::question(  this,
                                    tr("Error"),
                                    "Could not open the camera.",
                                    QMessageBox::Ok);


        }
}

void MyWindow::on_startCamera_clicked()
{
    if(camera_opend)
    {
        video_being_processed = true;
        ui->openCamera->setEnabled(false);
        ui->startCamera->setEnabled(false);
        ui->pauseCamera->setEnabled(true);
        ui->closeCamera->setEnabled(true);
        setThresh();
    }
    else
    {
        
    }
}

void MyWindow::on_pauseCamera_clicked()
{
    if(video_being_processed)
    {
        video_being_processed = false;
    }
    else
    {
        video_being_processed = true;
    }
}

void MyWindow::on_closeCamera_clicked()
{
    video_being_processed = false;
    camera_opend = false;
    cam.release();
    ui->openCamera->setEnabled(true);
    ui->startCamera->setEnabled(false);
    ui->pauseCamera->setEnabled(false);
    ui->closeCamera->setEnabled(false);
}

void MyWindow::on_HValue_slider_valueChanged(int value)
{
    std::ostringstream str;
    str<<value;
    ui->HValue_text->setText(QString(str.str().c_str()));
    setThresh();
}

void MyWindow::on_SValue_slider_valueChanged(int value)
{
    std::ostringstream str;
    str<<value;
    ui->SValue_text->setText(QString(str.str().c_str()));
    setThresh();
}

void MyWindow::on_VValue_slider_valueChanged(int value)
{
    std::ostringstream str;
    str<<value;
    ui->VValue_text->setText(QString(str.str().c_str()));
    setThresh();
}

void MyWindow::on_HThreshValue_slider_valueChanged(int value)
{
    std::ostringstream str;
    str<<value;
    ui->HThreshValue_text->setText(QString(str.str().c_str()));
    setThresh();
}

void MyWindow::on_SThreshValue_slider_valueChanged(int value)
{
    std::ostringstream str;
    str<<value;
    ui->SThreshValue_text->setText(QString(str.str().c_str()));
    setThresh();
}

void MyWindow::on_VThreshValue_slider_valueChanged(int value)
{
    std::ostringstream str;
    str<<value;
    ui->VThreshValue_text->setText(QString(str.str().c_str()));
    setThresh();
}

void MyWindow::on_blurSlider_valueChanged(int value){
    blurVal = value;
    std::cout << "The value of blur is :- " << blurVal << "\n";
    //    std::cout << "value changed!";
    std::ostringstream str;
    str << blurVal;
    ui->blurValLineEdit->setText(QString(str.str().c_str()));
    return;
}

void MyWindow::on_mblurSlider_valueChanged(int value){
    mblurVal = 2 * value + 1;
    std::cout << "The value of median is :- " << mblurVal << "\n";
    //    std::cout << "value changed!";
    std::ostringstream str;
    str << mblurVal;
    ui->mblurValLineEdit->setText(QString(str.str().c_str()));
    return;
}

void MyWindow::on_gblurSlider_valueChanged(int value){
    gblurVal = 2 * value + 1;
    std::cout << "The value of gblur is :- " << gblurVal << "\n";
    //    std::cout << "value changed!";
    std::ostringstream str;
    str << gblurVal;
    ui->gblurValLineEdit->setText(QString(str.str().c_str()));
    return;
}

void MyWindow::on_dilateCheck_clicked(){
    dilate_b = !dilate_b;

    std::cout << dilate_b;
}

void MyWindow::on_erodeCheck_clicked(){
    erode_b = !erode_b;

    std::cout << erode_b;
}

void MyWindow::on_morphellipse_clicked(){
    kernelShape = 0;
    std::cout << kernelShape;
}

void MyWindow::on_morphrect_clicked(){
    kernelShape = 1;
    std::cout << kernelShape;
}

void MyWindow::on_morphcross_clicked(){
    kernelShape = 2;
    std::cout << kernelShape;
}

void MyWindow::on_kernelSizeSlider_valueChanged(int value){
    kernelSizeVal = value;
    std::ostringstream str;
    str << value;
    ui->kernelSizeLineEdit->setText(QString(str.str().c_str()));
}

void MyWindow::on_contoursCheck_clicked(){
    draw_contours = !draw_contours;

    ui->boundrectCheck->setEnabled(draw_contours);
    ui->boundcircleCheck->setEnabled(draw_contours);
    ui->approxpolyCheck->setEnabled(draw_contours);
}

void MyWindow::on_boundrectCheck_clicked(){
    rectContours = !rectContours;
}

void MyWindow::on_boundcircleCheck_clicked(){
    circleContours = !circleContours;
}
void MyWindow::on_approxpolyCheck_clicked(){
    polyContours = !polyContours;
}

void MyWindow::on_houghCheck_clicked(){
    hough_enabled = !hough_enabled;
}

void MyWindow::on_houghMinRadius_valueChanged(int value){
    min_radius = value;

    std::ostringstream str;
    str << value;
    ui->houghMinRadiusLineEdit->setText(QString(str.str().c_str()));
}

void MyWindow::on_houghMaxRadius_valueChanged(int value){
    max_radius = value;

    std::ostringstream str;
    str << value;
    ui->houghMaxRadiusLineEdit->setText(QString(str.str().c_str()));
}

void MyWindow::on_houghLinesCheck_clicked(){
    hough_lines_enabled = !hough_lines_enabled;
}

void MyWindow::on_blobCheck_clicked(){
    blob_enabled = !blob_enabled;
}

void MyWindow::on_backgroundRSlider_valueChanged(int value){
    bg_r = value;

    std::ostringstream str;
    str << value;
    ui->backgroundRSliderLE->setText(QString(str.str().c_str()));

}

//void MyWindow::on_backgroundGSlider_valueChanged(int value){
//    bg_g = value;

//    std::ostringstream str;
//    str << value;
//    ui->backgroundGSliderLE->setText(QString(str.str().c_str()));

//}

//void MyWindow::on_backgroundBSlider_valueChanged(int value){
//    bg_b = value;

//    std::ostringstream str;
//    str << value;
//    ui->backgroundBSliderLE->setText(QString(str.str().c_str()));

//}

void MyWindow::on_foregroundRSlider_valueChanged(int value){
    fg_r = value;

    std::ostringstream str;
    str << value;
    ui->foregroundRSliderLE->setText(QString(str.str().c_str()));

}

//void MyWindow::on_foregroundGSlider_valueChanged(int value){
//    fg_g = value;

//    std::ostringstream str;
//    str << value;
//    ui->foregroundGSliderLE->setText(QString(str.str().c_str()));

//}

//void MyWindow::on_foregroundBSlider_valueChanged(int value){
//    fg_b = value;

//    std::ostringstream str;
//    str << value;
//    ui->foregroundBSliderLE->setText(QString(str.str().c_str()));

//}

void MyWindow::on_areablobSlider_valueChanged(int value){
    filter_arg5_area = value;

    std::ostringstream str;
    str << value;
    ui->areablobSliderLE->setText(QString(str.str().c_str()));
}

void MyWindow::on_areablobLess_clicked(){

    filter_arg4 = B_LESS;

}

void MyWindow::on_areablobMore_clicked(){
    filter_arg4 = B_GREATER;
}


void MyWindow::on_blobInclude_clicked(){
    filter_arg2 = B_INCLUDE;
}


void MyWindow::on_blobExclude_clicked(){
    filter_arg2 = B_EXCLUDE;
}

void MyWindow::on_colorImage_mouseDoubleClickEvent(QMouseEvent *ev)
{
    std::cout<<ev->x()<<" : "<<ev->y()<<std::endl;
    //uchar* data = colorImage.data;
    vec = thImage.at<cv::Vec3b>(ev->y(),ev->x());
    //    int H = data[ev->y()*colorImage.step1+ev->x()];
    //    int S = data[ev->y()*colorImage.step1+ev->x()+1];
    //    int V = data[ev->y()*colorImage.step1+ev->x()+2];
    ui->HValue_slider->setValue(vec[0]);
    ui->SValue_slider->setValue(vec[1]);
    ui->VValue_slider->setValue(vec[2]);
    std::cout<<"H:"<<(int)vec[0]<<"S:"<<(int)vec[1]<<"V:"<<(int)vec[2]<<std::endl;
    setThresh();
}

void MyWindow::on_updateImages()
{
    if(video_being_processed)
    {
        cam >> camImage;

        if(camImage.empty() && source_from_log){
            std::cout << "Entered the empty image branch!";

            cam.release();

            if(cam.open(fn_video.toUtf8().constData()))
                cam >> camImage;

            else
            {
                video_being_processed = false;
                camera_opend = false;
                cam.release();
                ui->openCamera->setEnabled(true);
                ui->startCamera->setEnabled(false);
                ui->pauseCamera->setEnabled(false);
                ui->closeCamera->setEnabled(false);
            }

        }

        cv::resize(camImage,colorImage,cv::Size(320,240));
        cv::cvtColor(colorImage,thImage,CV_RGB2HSV_FULL);

        using namespace cv;

        inRange(thImage,start,end,thImage);

        blur(thImage, thImage, Size(blurVal,blurVal));
        medianBlur(thImage,thImage,mblurVal);
        GaussianBlur(thImage, thImage, Size(gblurVal,gblurVal), 0);

        using namespace cv;

        Mat dilatekernel;

        if(kernelShape == 0)

            dilatekernel = getStructuringElement(MORPH_ELLIPSE, Size(kernelSizeVal, kernelSizeVal));

        if(kernelShape == 1)

            dilatekernel = getStructuringElement(MORPH_RECT, Size(kernelSizeVal, kernelSizeVal));

        if(kernelShape == 2)

            dilatekernel = getStructuringElement(MORPH_CROSS, Size(kernelSizeVal, kernelSizeVal));

        // First and second both argument selectable

        if(dilate_b)

            dilate(thImage, thImage, dilatekernel);

        if(erode_b)

            erode(thImage, thImage, dilatekernel);

        /////////////////////////////////////////////////////////////


        if(blob_enabled){

            IplImage Iipl = thImage;

            CBlobResult blobs, blobsclutter;
            CBlob * currentBlob;

            blobs = CBlobResult(&Iipl, NULL, bg_r);

            // The last parameter indicates the background. That is the color
            // on which we are searching for the clutter.

            // The last parameter in fillblob is the color with which we want to fill
            // the clutter that we found.

            blobs.Filter(blobs, filter_arg2, CBlobGetArea(), filter_arg4, filter_arg5_area);
            for (int i = 0; i < blobs.GetNumBlobs(); i++ )
            {
                currentBlob = blobs.GetBlob(i);
                currentBlob->FillBlob(&Iipl,Scalar(fg_r));
            }

            //        blobsclutter = CBlobResult(&Iipl, NULL, 0);
            //        blobsclutter.Filter(blobsclutter, B_INCLUDE, CBlobGetArea(), B_LESS, 500);
            //        for (int i = 0; i < blobsclutter.GetNumBlobs(); i++ )
            //        {
            //            currentBlob = blobsclutter.GetBlob(i);
            //            currentBlob->FillBlob(&Iipl,Scalar(0));
            //        }

        }

        binaryImage = thImage;

        Mat drawing;

        if(draw_contours){
            // Drawing contours

            int thresh = 100;
            RNG rng(12345);

            Mat threshold_output;
            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;

            threshold( thImage, threshold_output, thresh, 255, THRESH_BINARY );
            /// Find contours
            findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

            /// Approximate contours to polygons + get bounding rects and circles
            vector<vector<Point> > contours_poly( contours.size() );
            vector<Rect> boundRect( contours.size() );
            vector<Point2f>center( contours.size() );
            vector<float>radius( contours.size() );

            for( int i = 0; i < contours.size(); i++ )
            {
                approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
                boundRect[i] = boundingRect( Mat(contours_poly[i]) );
                minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
            }

            // Draw polygonal contour + bonding rects + circles
            drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
            for( int i = 0; i< contours.size(); i++ )
            {
                Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
                if(polyContours)
                    drawContours( colorImage, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
                if(rectContours)
                    rectangle( colorImage, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
                if(circleContours)
                    circle( colorImage, center[i], (int)radius[i], color, 2, 8, 0 );
            }

        }

        if(hough_enabled){

            vector<Vec3f> circles;
            /// Apply the Hough Transform to find the circles
            HoughCircles( thImage, circles, CV_HOUGH_GRADIENT, 2, thImage.rows/8, 200, 100, min_radius, max_radius);

            /// Draw the circles detected
            for( size_t i = 0; i < circles.size(); i++ )
            {
                Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
                int radius = cvRound(circles[i][2]);
                // circle center
                circle( colorImage, center, 3, Scalar(0,255,0), -1, 8, 0 );
                // circle outline
                circle( colorImage, center, radius, Scalar(0,0,255), 3, 8, 0 );
            }

        }

        if(hough_lines_enabled){

            vector<Vec4i> lines;
            HoughLinesP(thImage, lines, 1, CV_PI/180, 50, 50, 10 );
            for( size_t i = 0; i < lines.size(); i++ )
            {
                Vec4i l = lines[i];
                line( colorImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
            }

        }

        displayImages();
    }
}

void MyWindow::displayImages()
{
    /////////////////////////////////////////////////////////////////
    //cv::resize(image_raw,image_Qimage,cv::Size(400,275));
    cv::cvtColor(colorImage,displayImage,CV_BGR2RGB);
    QImage display1((uchar *)displayImage.data,displayImage.cols,
                    displayImage.rows,QImage::Format_RGB888);
    colorLabel->setPixmap(QPixmap::fromImage(display1));
    //////////////////////////////////////////////////////////////////
    //images[2].matrixToMat(image_processed);
    //cv::resize(image_processed,image_Qimage,cv::Size(400,275));
    //cv::cvtColor(image_Qimage,image_Qimage,CV_BGRA2RGB);
    QImage display2((uchar *)binaryImage.data,binaryImage.cols,
                    binaryImage.rows,QImage::Format_Indexed8);
    ui->binaryImage->setPixmap(QPixmap::fromImage(display2));
    //////////////////////////////////////////////////////////////////
}

MyWindow::~MyWindow()
{
    std::cout<<"deleting "<<std::endl;
}

void MyWindow::closeEvent(QCloseEvent *)
{
    std::cout<<"closing"<<std::endl;
    if(video_being_processed)
    {
        //on_Stop_clicked();
    }
    else
    {
    }
}
