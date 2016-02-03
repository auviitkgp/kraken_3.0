#include <ros/ros.h>
#include <bits/stdc++.h>
#include <task_buoy/buoy_server.h>
#define PI 3.1414

#include <resources/topicHeader.h>

int ***_allVals;
char* filepath;
bool canCall=false;
int p1,p2,p3,p4;
RNG rng(12345);

bool compar(const RotatedRect & a,const RotatedRect&b)
{
    return a.size.width*a.size.height >= b.size.width*b.size.height;
}

Buoy::Buoy(std::string name) : _it(_n), _s(_n, name, boost::bind(&Buoy::executeCB, this, _1), false), _actionName(name)
{
    _sub = _it.subscribe(topics::CAMERA_FRONT_RAW_IMAGE, 1, &Buoy::imageCallBack, this);
    _pub = _it.advertise(topics::CAMERA_FRONT_BUOY_IMAGE, 1);

    fstream f;
    f.open(filepath,ios::in);

    if (f)
    {
        for(int i=0; i<256; i++)
        {
            for (int j=0; j<256; j++)
            {
                for (int k=0; k<256; k++)
                {
                    if (!f.eof())
                    {
                        f>>_allVals[i][j][k];
                    }
                }
            }
        }
    }
    else
    {
        ROS_ERROR("Unable to open values file.");
        ros::shutdown();
    }

    _kernelDilateErode = getStructuringElement(MORPH_RECT, Size(3,3));
    //elementEx = getStructuringElement(MORPH_RECT, Size(7,7));
    _finalImage.encoding = "mono8";
    _s.start();
}

void Buoy::imageCallBack(const sensor_msgs::ImageConstPtr &_msg)
{
    cv_bridge::CvImagePtr _imagePtr;

    try
    {
        _imagePtr = cv_bridge::toCvCopy(_msg, "bgr8");
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", _msg->encoding.c_str());
    }

    _image = _imagePtr->image;
    canCall=true;
}

void Buoy::executeCB(const actionmsg::buoyGoalConstPtr &_goal)
{
    ros::Rate looprate(10);
    bool success = true;

    switch(_goal->order)
    {
        case DETECT_BUOY:
        {
            bool _detected = false;

            while(ros::ok())
            {
                if (_s.isPreemptRequested() || !ros::ok())
                {
                    ROS_INFO("%s: Preempted", _actionName.c_str());
                    _s.setPreempted();
                    success = false;
                    break;
                }
                if(!canCall){
                    looprate.sleep();
                    continue;
                }

                _detected = detectBuoy();
                canCall=false;
                _finalImage.image = _imageBW;
                _finalImagemsg = _finalImage.toImageMsg();
                _pub.publish(_finalImagemsg);

                if(_detected)
                {
                    _result.sequence.push_back(BUOY_DETECTED);
                    break;
                }

                looprate.sleep();
            }

            break;
        }

        case ALIGN_BUOY:
        {
            while(ros::ok())
            {
                if (_s.isPreemptRequested() || !ros::ok())
                {
                    ROS_INFO("%s: Preempted", _actionName.c_str());
                    _s.setPreempted();
                    success = false;
                    break;
                }

                detectBuoy();
                getAllignment();
                _finalImage.image = _imageBW;
                _finalImagemsg = _finalImage.toImageMsg();
                _pub.publish(_finalImagemsg);

                if((_feedback.errorx < 5 && _feedback.errorx > -5) && (_feedback.errory > -5 && _feedback.errory < 5))
                {
                    _result.sequence.push_back(BUOY_ALIGNED);
                    break;
                }

                _s.publishFeedback(_feedback);
                looprate.sleep();
            }

            break;
        }
    }

    if(success)
    {
        _result.sequence.push_back(_goal->order);
        _s.setSucceeded(_result);
    }
}

bool Buoy::detectBuoy()
{
    if(!_image.empty())
    {
        // cvtColor(_image, _imageHSV, CV_BGR2HSV);
        // inRange(_imageHSV,_lowerThreshRed1,_upperThreshRed1, _imageBW);
        // inRange(_imageHSV,_lowerThreshRed2,_upperThreshRed2, _imageBWRed);
        // add(_imageBW, _imageBWRed, _imageBW);
        // inRange(_imageHSV,_lowerThreshGreen,_upperThreshGreen, _imageBWGreen);
        // add(_imageBW, _imageBWGreen, _imageBW);
        int k;
        imshow("_imagetest1", _image);

        for (int j=0; j< _image.rows; j++)
        {
            for (int i=0; i< _image.cols; i++)
            {
                k = _allVals[_image.at<Vec3b>(j,i).val[0]][_image.at<Vec3b>(j,i).val[1]][_image.at<Vec3b>(j,i).val[2]];

                if (k==1)
                {
                    _image.at<Vec3b>(j,i).val[0] = 0;
                    _image.at<Vec3b>(j,i).val[1] = 0;
                    _image.at<Vec3b>(j,i).val[2] = 0;
                }

                if (k==0)
                {
                    _image.at<Vec3b>(j,i).val[0] = 255;
                    _image.at<Vec3b>(j,i).val[1] = 255;
                    _image.at<Vec3b>(j,i).val[2] = 255;
                }

                if (k==2)
                {
                    _image.at<Vec3b>(j,i).val[0] = 0;
                    _image.at<Vec3b>(j,i).val[1] = 0;
                    _image.at<Vec3b>(j,i).val[2] = 0;
                }
            }
        }
        //imshow("NoProcess", _image);
        
        erode(_image, _image, _kernelDilateErode);
        imshow("_imagetest", _image);
        cvtColor(_image, _imageBW, CV_BGR2GRAY);
        waitKey(33);
        medianBlur(_imageBW, _imageBW, 3);
        //erode(_imageBW, _imageBW, _kernelDilateErode);
        //imshow("_imageBW", _image);

        Mat exp_img = _imageBW.clone();

        findContours(exp_img.clone(), _contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
        drawContours(exp_img, _contours, -1, CV_RGB(255, 255, 255), -1);
        erode(exp_img, exp_img, _kernelDilateErode);
        findContours(exp_img.clone(), _contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
        drawContours(exp_img, _contours, -1, CV_RGB(255, 255, 255), -1);
        imshow("FILLED", exp_img);
        
        Mat temp_img = exp_img.clone();
        Canny(exp_img, exp_img, 50, 150);
        imshow("Experiment", exp_img);

        Mat src = temp_img.clone();
        src = Scalar(0,0,0);

        /*cv::SimpleBlobDetector blob_detector;
        vector<cv::KeyPoint> keypoints, keypoints1;
        blob_detector.detect(temp_img, keypoints);
        int i=0, j=0;
        for(;i<keypoints.size();i++)
        {
            cout << "HEEEE";
            if(keypoints[i].size > keypoints[j].size)
            {
                j = i;
                cout << j;
            }
        }
        /*for(std::vector<cv::KeyPoint>::iterator blobIterator = keypoints.begin(); blobIterator != keypoints.end(); blobIterator++, j++)
        {
            if( blobIterator->size > max_size)
            {
                max_size = blobIterator->size;
                i = j;
            }
        }
        keypoints1.clear();
        //keypoints1.push_back(keypoints[j]);
        cv::drawKeypoints(temp_img, keypoints, src);
        imshow("KeyPoint", src);
        //morphologyEx( _imageBW, _imageBW, MORPH_OPEN, elementEx );
        /*
        CBlobResult _blobs,_blobsClutter;
        CBlob * _currentBlob;
        IplImage _imageBWipl = _imageBW;
        _blobs = CBlobResult(&_imageBWipl, NULL, 0);
        _blobs.Filter(_blobs, B_INCLUDE, CBlobGetArea(), B_INSIDE, 50, 1000);
        _imageBW = Scalar(0, 0, 0);

        for(int i = 0; i < _blobs.GetNumBlobs(); i++)
        {
            _currentBlob = _blobs.GetBlob(i);
            _currentBlob->FillBlob(&_imageBWipl, Scalar(255));
        }

        Mat _imageBW2 = _imageBW.clone();
        vector<Mat> channels;
        channels.push_back(_imageBW);
        channels.push_back(_imageBW);
        channels.push_back(_imageBW);
        merge( channels, src);
        _contours.clear();
        medianBlur(_imageBW2, _imageBW2, 5);
        findContours(_imageBW2, _contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
        Point2f _centerBuff;
        float _radiusBuff;
        vector<Point> _contoursPolyBuff;
        _center.clear();
        _radius.clear();
        _contoursPoly.clear();
        _imageBW = Scalar(0, 0, 0);

        for(int i=0; i < _contours.size(); i++)
        {
            if(contourArea(_contours[i])>50)
            {
                approxPolyDP(_contours[i],_contoursPolyBuff,3,true);
                minEnclosingCircle((Mat)_contoursPolyBuff,_centerBuff,_radiusBuff);
                circle(_imageBW,_centerBuff,_radiusBuff,Scalar(255), -1);
                _center.push_back(_centerBuff);
                _radius.push_back(_radiusBuff);
                _contoursPoly.push_back(_contoursPolyBuff);
            }
        }

        Mat src_gray;
        cvtColor( src, src_gray, CV_BGR2GRAY );
        src = Scalar(0, 0, 0);
        GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );
        //imshow("src_gray", src_gray);*/
        vector<Vec3f> circles;
        circles.clear();

        vector<vector<Point> > contours;
        findContours(temp_img, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
        RotatedRect rotRecs[contours.size()];

        for (int i = 0; i < contours.size(); i++) {
            if(contours[i].size()>=5)
                rotRecs[i] = fitEllipse(contours[i]);
        }
        sort(rotRecs,rotRecs+contours.size(),compar);
        Mat threshold_output=temp_img.clone();
        Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
        
        for( int i = 0; i< 5; i++ )
        {
            if(rotRecs[i].size.width>0 && rotRecs[i].size.height>0){
               Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
               // contour
               // drawContours( drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
               // ellipse
               if(fabs(rotRecs[i].size.width-rotRecs[i].size.height)<=p1)
                ellipse( drawing, rotRecs[i], color, 2, 8 );
           }
           // rotated rectangle
           // Point2f rect_points[4]; minRect[i].points( rect_points );
           // for( int j = 0; j < 4; j++ )
           //    line( drawing, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );
        }
        imshow("drawing",drawing);

        /// Apply the Hough Transform to find the circles
        // HoughCircles( temp_img, circles, CV_HOUGH_GRADIENT, p1, p2,p3,p4);//1, 15, 5, 1000 
        //void HoughCircles(Input image, Output circles, int method, double dp, double minDist, double param1=100, double param2=100, int minRadius=0, int maxRadius=0 )

        // Draw the circles detected
        // if(circles.size() == 0)
        // {
        //     cout<<"NOTHING CIRCULAR" << endl;
        // }

        // for( size_t i = 0; i < circles.size(); i++ )
        // {
        //     Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        //     int radius = cvRound(circles[i][2]);
        //     cout << "radius = " << radius << "\n";
        //     // circle center
        //     circle( src, center, 3, Scalar(255, 255, 255), 3, 8, 0 );
        //     // circle outline
        //     circle( src, center, radius, Scalar(255, 255, 255), 1, 8, 0 );
        // }

        imshow("src", src);
        
        if(_center.size() > 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

void Buoy::getAllignment()
{
    for(int i = 0; i < _center.size(); i++)
    {
        _feedback.errorx = _image.cols/2 - _center[i].x;
        _feedback.errory = _image.rows/2 - _center[i].y;
        cout<< _feedback.errorx << " : " << _feedback.errory << endl;
    }
}

Buoy::~Buoy()
{
}

int main(int argc, char ** argv)
{
    if (argc < 2)
    {
        ROS_ERROR("You need to input the filepath to the RGB matrix file");
        return 0;
    }
    p1=atoi(argv[2]),p2=atoi(argv[3]);
    p3=atoi(argv[4]),p4=atoi(argv[5]);
    filepath = argv[1];

    _allVals = new int**[256];

    for(int i = 0; i < 256; i++)
    {
        _allVals[i] = new int*[256];

        for(int j = 0; j < 256; j++)
        {
            _allVals[i][j] = new int[256];
        }
    }

    ROS_INFO("Starting node");
    ros::init(argc, argv, "buoy_server_nn");
    Buoy _buoyserver("buoy_nn");
    ros::spin();
    return 0;
}
