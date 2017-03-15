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

vector<KalmanFilter> KFV;
vector<Mat_<float> > measurement(5,Mat_<float>(2,1));

bool comparxy(const RotatedRect & a,const RotatedRect&b)
{
    return a.center.x <= b.center.x;
}

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

    KFV.assign(5, KalmanFilter(4, 2, 0));

    for(int i=0;i<5;i++)
    {

        KFV[i].transitionMatrix = *(Mat_<float>(4, 4) << 1,0,1,0,   0,1,0,1,  0,0,1,0,  0,0,0,1);
        measurement[i].setTo(Scalar(0));
         
        // init...
        KFV[i].statePre.at<float>(0) = 0;
        KFV[i].statePre.at<float>(1) = 0;
        KFV[i].statePre.at<float>(2) = 0;
        KFV[i].statePre.at<float>(3) = 0;
        setIdentity(KFV[i].measurementMatrix);
        setIdentity(KFV[i].processNoiseCov, Scalar::all(1e-4));
        setIdentity(KFV[i].measurementNoiseCov, Scalar::all(1e-1));
        setIdentity(KFV[i].errorCovPost, Scalar::all(.1));
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
        int k;
        imshow("_imagetest1", _image);

        for (int j=0; j< _image.rows; j++)
        {
            for (int i=0; i< _image.cols; i++)
            {
                k = _allVals[_image.at<Vec3b>(j,i).val[0]][_image.at<Vec3b>(j,i).val[1]][_image.at<Vec3b>(j,i).val[2]];

                if (k==1)
                {
                    _image.at<Vec3b>(j,i).val[0] = 255;
                    _image.at<Vec3b>(j,i).val[1] = 255;
                    _image.at<Vec3b>(j,i).val[2] = 255;
                }

                if (k==0)
                {
                    _image.at<Vec3b>(j,i).val[0] = 0;
                    _image.at<Vec3b>(j,i).val[1] = 0;
                    _image.at<Vec3b>(j,i).val[2] = 0;
                }

                if (k==2)
                {
                    _image.at<Vec3b>(j,i).val[0] = 0;
                    _image.at<Vec3b>(j,i).val[1] = 0;
                    _image.at<Vec3b>(j,i).val[2] = 0;
                }
            }
        }
        
        erode(_image, _image, _kernelDilateErode);
        imshow("_imagetest", _image);
        cvtColor(_image, _imageBW, CV_BGR2GRAY);
        waitKey(33);
        medianBlur(_imageBW, _imageBW, 3);
        
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

        vector<Vec3f> circles;
        circles.clear();

        vector<vector<Point> > contours;
        findContours(temp_img, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
        RotatedRect rotRecs[contours.size()];

        for (int i = 0; i < contours.size(); i++)
        {
            if(contours[i].size()>=5)
                rotRecs[i] = fitEllipse(contours[i]);
        }
        sort(rotRecs,rotRecs+contours.size(),compar);
        Mat threshold_output=temp_img.clone();
        Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );

        for( int i = 0; i< 5 && i<contours.size(); i++ )
        {
            if(rotRecs[i].size.width>0 && rotRecs[i].size.height>0)
            {
                Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
                if(fabs(rotRecs[i].size.width-rotRecs[i].size.height)<=p1)
                    ellipse( drawing, rotRecs[i], color, 2, 8 );
            }
        }

        if(5<=contours.size())
            sort(rotRecs, rotRecs+5, comparxy);
        std::vector<Point> stPV(5);
        for(int i=0;i<5 && i<contours.size();i++)
        {
            Mat prediction = KFV[i].predict();
            Point predictPt(prediction.at<float>(0),prediction.at<float>(1));
                         
            // Get mouse point
            measurement[i](0) = rotRecs[i].center.x;
            measurement[i](1) = rotRecs[i].center.y;
                         
            Point measPt(measurement[i](0),measurement[i](1));
             
            // The "correct" phase that is going to use the predicted value and our measurement[i]
            Mat estimated = KFV[i].correct(measurement[i]);
            Point stP(estimated.at<float>(0),estimated.at<float>(1));
            stPV[i] = stP;
        }


        for( int i = 0; i< 5; i++ )
        {
            Point center = stPV[i];//(cvRound(circles[i][0]), cvRound(circles[i][1]));
            // int radius = 5;
            // cout << "radius = " << radius << "\n";
            // circle center
            circle( drawing, center, 5, Scalar(255, 255, 255), 3, 8, 0 );
            // circle outline
            // circle( src, center, radius, Scalar(255, 255, 255), 1, 8, 0 );
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
