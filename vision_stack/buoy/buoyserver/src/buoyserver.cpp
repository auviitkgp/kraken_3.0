#include <buoyserver/buoyserver.h>
#define PI 3.1414

Buoy::Buoy(std::string name) : _it(_n), _s(_n, name, boost::bind(&Buoy::executCB, this, _1), false), _actionName(name)
{
    _sub = _it.subscribe("videoimage", 1, &Buoy::imageCallBack, this);
    _pub = _it.advertise("bottomcamprocessed", 1);
    ifstream _thresholdVal("/home/madhukar/threshold.th");
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
        ROS_ERROR("Threshold file was not opened");
        ros::shutdown();
    }
    _kernelDilateErode = getStructuringElement(MORPH_RECT, Size(3,3));
    _finalImage.encoding = "bgr8";
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

void Buoy::executCB(const actionmsg::buoyGoalConstPtr &_goal)
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
                _finalImage.image = _image;
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
        case ALLIGN_BUOY:
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
                _finalImage.image = _image;
                _finalImagemsg = _finalImage.toImageMsg();
                _pub.publish(_finalImagemsg);
                if((_feedback.errorx < 10 && _feedback.errorx >> -10) && (_feedback.errory > -10 && _feedback.errory < 10))
                {
                    _result.sequence.push_back(BUOY_ALLIGNED);
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
        cvtColor(_image, _imageHSV, CV_BGR2HSV_FULL);
        inRange(_imageHSV, Scalar(0,0,0),Scalar(20,255,255), _imageBW);
        medianBlur(_imageBW, _imageBW, 3);
        erode(_imageBW, _imageBW, _kernelDilateErode);
        CBlobResult _blobs,_blobsClutter;
        CBlob * _currentBlob;
        IplImage _imageBWipl = _imageBW;
        _blobs = CBlobResult(&_imageBWipl, NULL, 0);
        _blobs.Filter(_blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 2000);
        for(int i = 0; i < _blobs.GetNumBlobs(); i++)
        {
            _currentBlob = _blobs.GetBlob(i);
            _currentBlob->FillBlob(&_imageBWipl, Scalar(0));
        }
        _blobsClutter = CBlobResult(&_imageBWipl, NULL, 0);
        _blobsClutter.Filter(_blobsClutter, B_EXCLUDE, CBlobGetArea(), B_GREATER, 50);
        for(int i = 0; i < _blobsClutter.GetNumBlobs(); i++)
        {
            _currentBlob = _blobsClutter.GetBlob(i);
            _currentBlob->FillBlob(&_imageBWipl, Scalar(0));
        }

        medianBlur(_imageBW, _imageBW, 5);
        findContours(_imageBW, _contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

        Point2f _centerBuff;
        float _radiusBuff;
        vector<Point> _contoursPolyBuff;
        _center.clear();
        _radius.clear();
        _contoursPoly.clear();

        for(int i=0; i < _contours.size(); i++)
        {
            if(contourArea(_contours[i])>100)
            {
               approxPolyDP(_contours[i],_contoursPolyBuff,3,true);
               minEnclosingCircle((Mat)_contoursPolyBuff,_centerBuff,_radiusBuff);
               circle(_image,_centerBuff,_radiusBuff,Scalar(255,0,0));
               _center.push_back(_centerBuff);
               _radius.push_back(_radiusBuff);
               _contoursPoly.push_back(_contoursPolyBuff);
            }
        }

        if(_center.size() > 0)
            return true;
        else
            false;
    }
    else
        false;
}

void Buoy::getAllignment()
{
    for(int i = 0; i < _center.size(); i++)
    {
        _feedback.errorx = _image.rows/2 - _center[i].x;
        _feedback.errory = _image.cols/2 - _center[i].y;
    }
}

Buoy::~Buoy()
{

}
