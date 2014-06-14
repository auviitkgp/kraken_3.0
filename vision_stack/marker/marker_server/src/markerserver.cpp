#include <marker_server/markerserver.h>

Marker::Marker(std::string name) : _it(_n), _s(_n, name, boost::bind(&Marker::executCB, this, _1), false), _actionName(name)
{
    _sub = _it.subscribe("bottomcamimage", 1, &Marker::imageCallBack, this);
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


void Marker::imageCallBack(const sensor_msgs::ImageConstPtr &_msg)
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
    I = _imagePtr->image;
}

void Marker::executCB(const actionmsg::markerGoalConstPtr &_goal)
{
    ros::Rate looprate(10);
    bool success = true;

    switch(_goal->order)
    {
        case DETECT_MARKER:
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
                detectMarker();
                _finalImage.image = I;
                _finalImagemsg = _finalImage.toImageMsg();
                _pub.publish(_finalImagemsg);

                looprate.sleep();
            }
            break;
        }
        case ALLIGN_MARKER:
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
                detectMarker();
                getAllignment();
                _finalImage.image = I;
                _finalImagemsg = _finalImage.toImageMsg();
                _pub.publish(_finalImagemsg);
                if(_feedback.errorangle == 0)
                {
                    _result.sequence.push_back(MARKER_ALLIGNED);
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

void Marker::detectMarker()
{
    cvtColor(I, I_hsv, CV_BGR2HSV_FULL);
    inRange(I_hsv, _lowerThresh, _upperThresh, I_bw);
    medianBlur(I_bw, I_bw, 3);
    erode(I_bw, I_bw, _kernelDilateErode);

    CBlobResult _blobs, _blobsClutter;
    CBlob * _currentBlob;

    IplImage _I_bw_ipl = I_bw;

    _blobs = CBlobResult(&_I_bw_ipl, NULL, 0);
    _blobs.Filter(_blobs, B_EXCLUDE, CBlobGetArea(), B_GREATER, 1000);

    _blobsClutter=CBlobResult(&_I_bw_ipl,NULL,255);
    _blobsClutter.Filter(_blobsClutter, B_EXCLUDE, CBlobGetArea(), B_GREATER, 1000);

    for(int i = 0; i < _blobs.GetNumBlobs(); i++)
    {
        _currentBlob = _blobs.GetBlob(i);
        _currentBlob->FillBlob(&_I_bw_ipl, Scalar(0));
    }

    for (int i = 0; i < _blobsClutter.GetNumBlobs(); i++)
    {
        _currentBlob = _blobsClutter.GetBlob(i);
        _currentBlob->FillBlob(&_I_bw_ipl,Scalar(255));
    }

    vector<vector<Point> > _contours;
    _minRect.clear();

    findContours(I_bw, _contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    for(int i=0; i < _contours.size(); i++)
    {
        if(contourArea(_contours[i])>2000)
        {
           _minRect.push_back(minAreaRect( Mat(_contours[i])));
        }
    }
}

void Marker::getAllignment()
{
    for( int i = 0; i< _minRect.size(); i++ )
    {
        Scalar _color = Scalar(  Scalar(0,0,255) );
        Point2f _rectPoints[4];
        _minRect[i].points( _rectPoints );
        for( int j = 0; j < 4; j++ )
        {
            line( I, _rectPoints[j], _rectPoints[(j+1)%4], _color, 1, 8 );
            cout << _minRect[i].center.x << ";" << _minRect[i].center.y << endl;
            cout << _minRect[i].angle << endl;
            _feedback.errorx = I.rows/2 - _minRect[i].center.x;
            _feedback.errorangle = 180 - _minRect[i].angle;
        }
    }
}

Marker::~Marker()
{
}
