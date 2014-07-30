#include <ros/ros.h>
#include <task_marker/marker_server.h>

#include <resources/topicHeader.h>

Marker::Marker(std::string name, std::string _threshold_filepath) : _it(_n), _s(_n, name, boost::bind(&Marker::executeCB, this, _1), false), _actionName(name)
{
    _sub = _it.subscribe(topic::CAMERA_BOTTOM_RAW_IMAGE, 1, &Marker::imageCallBack, this);
    // _pub = _it.advertise("/kraken/bottomcam/marker_image", 1);
    _pub = _it.advertise(topics::CAMERA_BOTTOM_MARKER_IMAGE, 1);
    marker_detect_status=false;
    marker_align_status=false;

    ifstream _thresholdVal(_threshold_filepath.c_str());
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
        ROS_ERROR("marker_server : Unable to open threshold file.");
        ros::shutdown();
    }
    _kernelDilateErode = getStructuringElement(MORPH_RECT, Size(3,3));
    _finalImage.encoding = "mono8";
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
        ROS_ERROR("marker_server : Could not convert from '%s' to 'bgr8'.", _msg->encoding.c_str());
    }
    I = _imagePtr->image;
}

void Marker::executeCB(const ip_msgs::markerGoalConstPtr &_goal)
{
    ros::Rate looprate(10);
    bool success = true;

    switch(_goal->order)
    {
        case DETECT_MARKER:
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

                _finalImage.image = I_bw;
                _finalImagemsg = _finalImage.toImageMsg();
                _pub.publish(_finalImagemsg);
                if(marker_detect_status)
                    break;
                looprate.sleep();
            }
            break;
        case ALIGN_MARKER:
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
                _finalImage.image = I_bw;
                _finalImagemsg = _finalImage.toImageMsg();
                _pub.publish(_finalImagemsg);
                
                _s.publishFeedback(_feedback);
                if(marker_align_status)
                    break;
                looprate.sleep();
            }
            break;

    }

    if(success)
    {
        ROS_INFO("goal succeeded");
        _result.sequence.push_back(_goal->order);
        _s.setSucceeded(_result);

    }
    else{
        _s.setPreempted();
    }
}

void Marker::detectMarker()
{
    cvtColor(I, I_hsv, CV_BGR2HSV);
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

    Mat I_bw2 = I_bw.clone();
    findContours(I_bw2, _contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    for(int i=0; i < _contours.size(); i++)
    {
        if(contourArea(_contours[i])>400)
        {
           _minRect.push_back(minAreaRect( Mat(_contours[i])));
            marker_detect_status=true;
        }
    }
}

void Marker::getAllignment()
{
    for( int i = 0; i< _minRect.size(); i++ )
    {
        Scalar _color = Scalar(  Scalar(255,0,0) );
        Point2f _rectPoints[4];
        _minRect[i].points( _rectPoints );
        for( int j = 0; j < 4; j++ )
        {
            line( I_bw, _rectPoints[j], _rectPoints[(j+1)%4], _color, 3, 8 );
            // cout << "[" << _minRect[i].center.x << ";" << _minRect[i].center.y << "]" << "[" << _minRect[i].angle << "]" << endl;
            _feedback.errorx = I_bw.rows/2 - _minRect[i].center.x;
            _feedback.errorangle = _minRect[i].angle;
            cout << "[" << _feedback.errorx << " -- " << _feedback.errorangle << "]" << endl;

            if(_feedback.errorangle < 3 && _feedback.errorangle > -3 && _feedback.errorx < 5)
                marker_align_status = true;
        }
    }
}

Marker::~Marker()
{
}

int main(int argc, char ** argv)
{
    ros::init(argc, argv, "markerserver");
    if(argc < 2)
    {
        ROS_INFO("marker_server : Requires threshold file as input parameter");
        ros::shutdown();
    }
    Marker _markerAction("marker", argv[1]);
    ros::spin();
    return 0;
}