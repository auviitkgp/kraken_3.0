#include <vgateserver/vgateserver.h>
#define PI 3.1414
#include <resources/topicHeader.h>

Vgate::Vgate(std::string name) : _it(_n), _s(_n, name, boost::bind(&Vgate::executCB, this, _1), false), _actionName(name)
{
    _sub = _it.subscribe(topics::CAMERA_BOTTOM_RAW_IMAGE, 1, &Vgate::imageCallBack, this);
    _pub = _it.advertise(topics::CAMERA_BOTTOM_VGATE_IMAGE, 1);
    
    ifstream _thresholdVal("threshold.th");
    if(_thresholdVal.is_open())
    {
        for(int i = 0; i < 3; i++)
        {
            _thresholdVal >> _lowerThresh[i];
        }
        for(int i = 0; i < 3; i++)
        {
            _thresholdVal >> _upperThresh[i];
        }
    }
    else
    {
        ROS_ERROR("Unable to open threshold.th file.");
        ros::shutdown();
    }
    _kernelDilateErode = getStructuringElement(MORPH_RECT, Size(3,3));
    _finalImage.encoding = "bgr8";
    _s.start();
}

void Vgate::imageCallBack(const sensor_msgs::ImageConstPtr &_msg)
{
    cv_bridge::CvImagePtr _imagePtr;
    try
    {
        _imagePtr = cv_bridge::toCvCopy(_msg, "bgr8");
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("validationgate : Could not convert from '%s' to 'bgr8'.", _msg->encoding.c_str());
    }
    _image = _imagePtr->image;
}

float Vgate::angleWrtY(const Point2f &v1, const Point2i &v2)
{
    Point2f v3;
    v3.x = v1.x - v2.x;
    v3.y = v1.y - v2.y;
    float angle = atan2(v3.y, v3.x);
    return angle*180/PI;
}

Point2i Vgate::rotatePoint(const Point2i &v1, const Point2i &v2, float angle)
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

void Vgate::executCB(const actionmsg::vgateGoalConstPtr &_goal)
{
    ros::Rate looprate(10);
    bool success = true;

    switch(_goal->order)
    {
        case DETECT_VGATE:
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
                _detected = detectVgate();
                _finalImage.image = _image;
                _finalImagemsg = _finalImage.toImageMsg();
                _pub.publish(_finalImagemsg);

                if(_detected)
                {
                    break;
                }
                looprate.sleep();
            }
            break;
        }
        case ALLIGN_VGATE:
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
                detectVgate();
                getAllignment();
                _finalImage.image = _image;
                _finalImagemsg = _finalImage.toImageMsg();
                _pub.publish(_finalImagemsg);
                if((_feedback.errorx < 10 && _feedback.errorx >> -10) && (_feedback.errory > -10 && _feedback.errory < 10))
                {
                    _result.sequence.push_back(VGATE_ALLIGNED);
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

bool Vgate::detectVgate()
{
    if(!_image.empty())
    {
        cvtColor(_image, _imageHSV, CV_BGR2HSV_FULL);
        inRange(_imageHSV, _lowerThresh, _upperThresh, _imageBW);
        medianBlur(_imageBW, _imageBW, 5);
        Canny(_imageBW, _imageBW, 50, 200);


        HoughLinesP(_imageBW, _lines, 1, CV_PI/180, 100, 75, 40);
        if(_lines.size() >= 4)
            return true;
        else
            return false;
    }
    else
        return false;
}

void Vgate::getAllignment()
{
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
    line(_image, _coordinate[0], _coordinate[2], Scalar(255,0,0),3,8);
    _rodB.x=(_coordinate[0].x + _coordinate[2].x)/2;
    _rodB.y=(_coordinate[0].y + _coordinate[2].y)/2;
    _feedback.errorx = _image.rows/2 - _rodB.x;
    _feedback.errory = _image.cols/2 - _rodB.y;
}

Vgate::~Vgate()
{
}
