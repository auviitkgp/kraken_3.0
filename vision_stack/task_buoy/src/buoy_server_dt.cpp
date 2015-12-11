#include <ros/ros.h>
#include <iostream>
#include <task_buoy/buoy_server.h>
#define PI 3.1414

#include <resources/topicHeader.h>

Buoy::Buoy(std::string name) : _it(_n), _s(_n, name, boost::bind(&Buoy::executeCB, this, _1), false), _actionName(name)
{
    _sub = _it.subscribe(topics::CAMERA_FRONT_RAW_IMAGE, 1, &Buoy::imageCallBack, this);
    _pub = _it.advertise(topics::CAMERA_FRONT_BUOY_IMAGE, 1);

    /*fstream f;
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
    }*/

    std::cout << "Before training";

    string fname="final_data_file",word,wordred;
    string fname_red="final_data_file_red";
    
    std::ifstream nameFileout;
    nameFileout.open(fname.c_str());
    std::getline(nameFileout, word);
    int c=0;
    for (int i = 0; i < 256; ++i){
        for (int j = 0; j < 256; ++j){
            for (int k = 0; k < 256; ++k){
                helper[i][j][k]=word[c++]=='1'?1:0;
            }
        }
    }
    freopen(fname_red.c_str(),"r",stdin);
    cin>>wordred;
    c=0;
    for (int i = 0; i < 256; ++i){
        for (int j = 0; j < 256; ++j){
            for (int k = 0; k < 256; ++k){
                helper_red[i][j][k]=wordred[c++]=='1'?1:0;
            }
        }
    }

    ROS_INFO("After training");

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

                _detected = detectBuoy();
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
    

    /*VideoCapture cap("/media/prudhvi/Ubuntu1204/home/prudhvi/Videos/AUV_videos/gate_detection.avi"); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    */

    Mat hsv_image;
    
        //Mat frame;
        //cap >> frame; // get a new frame from camera
        
        cvtColor(_image, hsv_image, CV_BGR2HSV);

        cv::Mat res (hsv_image.size(), CV_8UC3);
        uint8_t *resdata = res.data;


        uint8_t *hsv_data = (uint8_t*)(hsv_image.data);
        
        for (int j = 0; j < hsv_image.rows; ++j){
            for (int i = 0; i < hsv_image.cols; ++i){   
                int h=hsv_image.at<cv::Vec3b>(j,i)[0];
                int s=hsv_image.at<cv::Vec3b>(j,i)[1];
                int v=hsv_image.at<cv::Vec3b>(j,i)[2];

                res.at<cv::Vec3b>(j,i)[0]=helper[h][s][v]*255;
                res.at<cv::Vec3b>(j,i)[1]=helper[h][s][v]*255;
                res.at<cv::Vec3b>(j,i)[2]=0;
                // if(helper_red[h][s][v]==1){
                //     res.at<cv::Vec3b>(j,i)[0]=0;
                //     res.at<cv::Vec3b>(j,i)[1]=0;
                //     res.at<cv::Vec3b>(j,i)[2]=helper_red[h][s][v]*255;
                // }
            }
        }

        // Mat res_gray;
        // cvtColor(res, res_gray, CV_BGR2GRAY);

        // GaussianBlur( res_gray, res_gray, Size(9, 9), 2, 2 );

        // vector<Vec3f> circles;

        // /// Apply the Hough Transform to find the circles
        // HoughCircles( res_gray, circles, CV_HOUGH_GRADIENT, 1, res_gray.rows/8, 200, 100, 0, 0 );
        // printf("%d circles found\n",circles.size() );
        // /// Draw the circles detected
        // for( size_t i = 0; i < circles.size(); i++ )
        // {
        //   Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        //   int radius = cvRound(circles[i][2]);
        //   // circle center
        //   circle( res, center, 3, Scalar(0,255,0), -1, 8, 0 );
        //   // circle outline
        //   circle( res, center, radius, Scalar(0,0,255), 3, 8, 0 );
        // }
        // imshow("gray",res_gray);
        imshow("result",res);
        imshow("frame",_image);




    /*if(!_image.empty())
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
                    _image.at<Vec3b>(j,i).val[1] = 255;
                    _image.at<Vec3b>(j,i).val[2] = 255;
                }

                if (k==0)
                {
                    _image.at<Vec3b>(j,i).val[0] = 0;
                    _image.at<Vec3b>(j,i).val[1] = 0;
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

        imshow("_imagetest", _image);
        cvtColor(_image, _imageBW, CV_BGR2GRAY);
        imshow("_imageBW", _imageBW);
        waitKey(33);
        medianBlur(_imageBW, _imageBW, 3);
        erode(_imageBW, _imageBW, _kernelDilateErode);
        //morphologyEx( _imageBW, _imageBW, MORPH_OPEN, elementEx );
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
        Mat src;
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
        imshow("src_gray", src_gray);
        vector<Vec3f> circles;
        /// Apply the Hough Transform to find the circles
        HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/16, 150, 25, 0, 0 );

        /// Draw the circles detected
        if(circles.size() == 0)
        {
            cout<<"NOTHING CIRCULAR" << endl;
        }

        for( size_t i = 0; i < circles.size(); i++ )
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            // circle center
            circle( src, center, 3, Scalar(0, 255, 0), 3, 8, 0 );
            // circle outline
            circle( src, center, radius, Scalar(0, 0, 255), 1, 8, 0 );
        }

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
    }*/
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
    
    ROS_INFO("From main");
    
    ros::init(argc, argv, "buoy_server");
    ROS_INFO("From main");
    Buoy _buoyserver("buoy");
    ros::spin();
    return 0;
}
