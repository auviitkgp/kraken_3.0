#include <ros/ros.h>
#include <iostream>
#include <task_buoy/buoy_server.h>
#define PI 3.1414

#include <resources/topicHeader.h>

int helper[256][256][256], helper_red[256][256][256];

Buoy::Buoy(std::string name) : _it(_n), _s(_n, name, boost::bind(&Buoy::executeCB, this, _1), false), _actionName(name)
{
    _sub = _it.subscribe(topics::CAMERA_FRONT_RAW_IMAGE, 100, &Buoy::imageCallBack, this);
    _pub = _it.advertise(topics::CAMERA_FRONT_BUOY_IMAGE, 100);

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

    for (int i = 0; i < 256; ++i)
    {
        for (int j = 0; j < 256; ++j)
        {
            for (int k = 0; k < 256; ++k)
            {
                helper[i][j][k]=word[c++]=='1'?1:0;
            }
        }
    }

    freopen(fname_red.c_str(),"r",stdin);
    cin>>wordred;
    c=0;

    for (int i = 0; i < 256; ++i)
    {
        for (int j = 0; j < 256; ++j)
        {
            for (int k = 0; k < 256; ++k)
            {
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
    ros::Rate looprate(5);
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

    cvtColor(_image, hsv_image, CV_BGR2HSV);

    cv::Mat res (hsv_image.size(), CV_8UC3);
    uint8_t *resdata = res.data;


    uint8_t *hsv_data = (uint8_t*)(hsv_image.data);

    for (int j = 0; j < hsv_image.rows; ++j)
    {
        for (int i = 0; i < hsv_image.cols; ++i)
        {
            int h=hsv_image.at<cv::Vec3b>(j,i)[0];
            int s=hsv_image.at<cv::Vec3b>(j,i)[1];
            int v=hsv_image.at<cv::Vec3b>(j,i)[2];

            res.at<cv::Vec3b>(j,i)[0]=helper[h][s][v]*255;
            res.at<cv::Vec3b>(j,i)[1]=helper[h][s][v]*255;
            res.at<cv::Vec3b>(j,i)[2]=0;

            if(helper_red[h][s][v]==1)
            {
                res.at<cv::Vec3b>(j,i)[0]=0;
                res.at<cv::Vec3b>(j,i)[1]=0;
                res.at<cv::Vec3b>(j,i)[2]=helper_red[h][s][v]*255;
            }
        }
    }

    imshow("result",res);
    imshow("frame",_image);
    cv::waitKey(33);

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

    ros::init(argc, argv, "buoy_server_dt");
    ROS_INFO("From main");
    Buoy _buoyserver("buoy_dt");
    ros::spin();
    return 0;
}
