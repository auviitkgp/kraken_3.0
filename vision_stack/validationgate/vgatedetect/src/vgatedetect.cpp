#include <iostream>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <fstream>
#define PI 3.1414

using namespace std;
using namespace cv;

Mat _image;

void imageCallBack(const sensor_msgs::ImageConstPtr &_msg)
{
    cv_bridge::CvImagePtr _imagePtr;
    try
    {
        _imagePtr = cv_bridge::toCvCopy(_msg, "bgr8");
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("vgatedetect : Could not convert from '%s' to 'bgr8'.", _msg->encoding.c_str());
    }
    _image = _imagePtr->image;
}

float angleWrtY(const Point2f &v1, const Point2i &v2)
{
    Point2f v3;
    v3.x = v1.x - v2.x;
    v3.y = v1.y - v2.y;
    float angle = atan2(v3.y, v3.x);
    return angle*180/PI;
}

Point2i rotatePoint(const Point2i &v1, const Point2i &v2, float angle)
{
    if(v1.x > v2.x)
    {
        Point2i v3 = v1 - v2;
        Point2i finalVertex;
        finalVertex.x = v3.x * cos(angle) - v3.y * sin(angle);
        finalVertex.y = v3.x * sin(angle) + v3.y * cos(angle);
        finalVertex = finalVertex + v2;
        return finalVertex;
    }
    else
    {
        Point2i v3 = v2 - v1;
        Point2i finalVertex;
        finalVertex.x = v3.x * cos(angle) - v3.y * sin(angle);
        finalVertex.y = v3.x * sin(angle) + v3.y * cos(angle);
        finalVertex = finalVertex + v1;
        return finalVertex;
    }
}

int main(int argc, char ** argv)
{
	ros::init(argc, argv, "vgatedetect");

    ros::NodeHandle _n;
    image_transport::ImageTransport _it(_n);
    image_transport::Subscriber _sub = _it.subscribe("videoimage", 1, imageCallBack);

    Mat _imageHSV, _imageBW;

    Scalar _lowerThresh(60, 0, 0),_upperThresh(105, 255, 255);

    vector<Vec4i> _lines;

    ros::Rate _looprate(10);

    while(ros::ok())
    {
        if(_image.empty())
            ROS_ERROR("vgatedetect : The image is empty");
        else
        {
            cvtColor(_image, _imageHSV, CV_BGR2HSV_FULL);
            inRange(_imageHSV, _lowerThresh, _upperThresh, _imageBW);
            medianBlur(_imageBW, _imageBW, 5);
            Canny(_imageBW, _imageBW, 50, 200);


            HoughLinesP(_imageBW, _lines, 1, CV_PI/180, 100, 75, 40);

            Point2i _coordinate[4], _rodB;

            for( int i = 0; i < _lines.size(); i++ )
            {
                line(_image, Point2i(_lines[i][0], _lines[i][1]), Point2i(_lines[i][2], _lines[i][3]), Scalar(255,255,0),3,8);
                if(i==0)
                {
                    _coordinate[0].x = _lines[i][0];			//taking min x
                    _coordinate[0].y = _lines[i][1];			//taking y corresponding to min x
                    _coordinate[1].x = _lines[i][2];			//taking max x
                    _coordinate[1].y = _lines[i][3];            //taking y corresponding to max x
                    _coordinate[2].x = _lines[i][0];            //taking x corresponding to min y
                    _coordinate[2].y = _lines[i][1];            //taking min y
                    _coordinate[3].x = _lines[i][2];            //taking x corresponding to max y
                    _coordinate[3].y = _lines[i][3];            //taking max y
                }
                else
                {
                    if(_lines[i][0] < _coordinate[0].x)
                    {
                        _coordinate[0].x = _lines[i][0];
                        _coordinate[0].y = _lines[i][1];
                    }
                    if(_lines[i][2] > _coordinate[1].x)
                    {
                        _coordinate[1].x = _lines[i][2];
                        _coordinate[1].y = _lines[i][3];
                    }
                    if(_lines[i][0] > _coordinate[1].x)
                    {
                        _coordinate[1].x = _lines[i][0];
                        _coordinate[1].y = _lines[i][1];
                    }
                    if(_lines[i][2] < _coordinate[0].x)
                    {
                        _coordinate[0].x = _lines[i][2];
                        _coordinate[0].y = _lines[i][3];
                    }
                    if(_lines[i][1] < _coordinate[2].y)
                    {
                        _coordinate[2].x = _lines[i][0];
                        _coordinate[2].y = _lines[i][1];
                    }
                    if(_lines[i][3] > _coordinate[3].y)
                    {
                        _coordinate[3].x = _lines[i][2];
                        _coordinate[3].y = _lines[i][3];
                    }
                    if(_lines[i][1] > _coordinate[3].y)
                    {
                        _coordinate[3].x = _lines[i][0];
                        _coordinate[3].y = _lines[i][1];
                    }
                    if(_lines[i][3] < _coordinate[2].y)
                    {
                        _coordinate[2].x = _lines[i][2];
                        _coordinate[2].y = _lines[i][3];
                    }
                }
            }
            float angle = angleWrtY(_coordinate[0], _coordinate[2]);
            if((angle < 25) ||(angle > 155) ) // Check the angle range
            {
                _coordinate[2] = rotatePoint(_coordinate[0], _coordinate[2], -PI/4);
            }
            else
            {
                cout << _image.rows/2 - _rodB.x << endl;
                cout << _image.cols/2 - _rodB.y << endl;
            }

            line(_image, _coordinate[0], _coordinate[2], Scalar(255,0,0),3,8);
            _rodB.x=(_coordinate[0].x + _coordinate[2].x)/2;
            _rodB.y=(_coordinate[0].y + _coordinate[2].y)/2;
            imshow("vgatedetect : Final Image", _image);
            if(waitKey(33) == 27)
                break;
        }
        _looprate.sleep();
        ros::spinOnce();
    }
	return 0;
}
