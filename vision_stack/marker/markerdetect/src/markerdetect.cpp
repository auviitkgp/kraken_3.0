#include <iostream>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <blob/blob.h>
#include <blob/BlobResult.h>
#include <blob/BlobContour.h>
#include <blob/BlobLibraryConfiguration.h>
#include <blob/BlobOperators.h>
#include <blob/ComponentLabeling.h>
#include <blob/BlobProperties.h>

using namespace std;
using namespace cv;

Mat _I;

void imageCallback(const sensor_msgs::ImageConstPtr& _msg)
{
    cv_bridge::CvImagePtr _cvptr;
    try
    {
        _cvptr = cv_bridge::toCvCopy(_msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    _I = _cvptr->image;
}

int main(int argc, char ** argv)
{
	ros::init(argc, argv, "markerDetect");

    VideoCapture _camera;
    ros::NodeHandle _n;
    image_transport::ImageTransport _it(_n);
    image_transport::Subscriber _sub;

    ros::Rate looprate(10);

    bool _capture = true;

    if(argc >= 2)
    {
        if(argv[1] == std::string("stream"))
        {
            _sub = _it.subscribe("imageStream", 1, imageCallback);
            _capture = false;
            cout << "Inside";
        }
        else
        {
            if(_camera.open(atoi(argv[1])))
                ROS_INFO("The camera opened successfully\n");
            else
            {
                ROS_ERROR("The camera was not able to open");
                return 0;
            }
        }
    }

    else
    {
        if(_camera.open("/home/madhukar/videolog/downward-pipe-15_56_17.avi"))
            ROS_INFO("The camera opened successfully\n");
        else
            ROS_ERROR("The camera was not able to open");
    }

    Mat _ihsv, _ithresh;
    Scalar _lowerThresh(0, 0, 0),_upperThresh(20, 255, 255);

    Mat _dilateKernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    CBlobResult _blobs, _blobsclutter;
    CBlob * _currentBlob;

    while(ros::ok())
    {
        if(_capture == true)
            _camera >> _I;

        if(_I.empty())
        {
            cout << "Image is Empty";
        }

        else
        {
            cvtColor(_I, _ihsv, CV_BGR2HSV_FULL);
            inRange(_ihsv, _lowerThresh, _upperThresh, _ithresh);

            medianBlur(_ithresh, _ithresh, 5);

            erode(_ithresh, _ithresh, _dilateKernel);

            IplImage _iipl = _ithresh;

            _blobs = CBlobResult(&_iipl, NULL, 255);
            _blobs.Filter(_blobs, B_INCLUDE, CBlobGetArea(), B_LESS, 500);
            for (int i = 0; i < _blobs.GetNumBlobs(); i++ )
            {
                _currentBlob = _blobs.GetBlob(i);
                _currentBlob->FillBlob(&_iipl,Scalar(255));
            }
            _blobsclutter = CBlobResult(&_iipl, NULL, 0);
            _blobsclutter.Filter(_blobsclutter, B_INCLUDE, CBlobGetArea(), B_LESS, 500);
            for (int i = 0; i < _blobsclutter.GetNumBlobs(); i++ )
            {
                _currentBlob = _blobsclutter.GetBlob(i);
                _currentBlob->FillBlob(&_iipl,Scalar(0));
            }

            vector<vector<Point> > _contours;
            vector<RotatedRect> _minRect;

            findContours(_ithresh, _contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

            for(int i=0; i < _contours.size(); i++)
            {
                if(contourArea(_contours[i])>2000)
                {
                   _minRect.push_back(minAreaRect( Mat(_contours[i])));
                }
            }

            for( int i = 0; i< _minRect.size(); i++ )
            {
                Scalar _color = Scalar(  Scalar(0,0,255) );
                Point2f _rectPoints[4];
                _minRect[i].points( _rectPoints );
                for( int j = 0; j < 4; j++ )
                {
                    line( _I, _rectPoints[j], _rectPoints[(j+1)%4], _color, 1, 8 );
                    cout << _minRect[i].center.x << ";" << _minRect[i].center.y << endl;
                    cout << _minRect[i].angle << endl;
                }
            }

        }

        imshow("Processed Image", _I);

        ros::spinOnce();
        looprate.sleep();

        if(waitKey(33) == 27)
            break;
    }

	return 0;
}
