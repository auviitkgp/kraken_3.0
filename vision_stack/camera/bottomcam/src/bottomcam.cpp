#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <std_msgs/String.h>
#include <resources/topicHeader.h>

using namespace std;
using namespace cv;

int camMsg = 0;  // 0 : front cam   1:  bottom cam
int cameraNo = 1;

bool camOpen = false;
int flag = 0;

VideoCapture cam;

void msgCallback(const std_msgs::String::ConstPtr& msg)
{
    camMsg = atoi(msg->data.c_str());
    if(camMsg == cameraNo && !camOpen)
    {
        if(cam.open(camMsg))
        {
            cout << "BottomCam opened successfully." << endl;
            camOpen = true;
        }
    }
    else if(camMsg != cameraNo && camOpen)
    {
        cam.release();
        cout << "BottomCam closed successfully." << endl;
        camOpen = false;
    }
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "bottomcam");

    ros::NodeHandle _nh;
    image_transport::ImageTransport _it(_nh);
    image_transport::Publisher _image_pub = _it.advertise(topics::CAMERA_BOTTOM_RAW_IMAGE, 1);
    ros::Subscriber _sub = _nh.subscribe(topics::CAMERA_CAM_SWITCH, 1, msgCallback);
    sensor_msgs::ImagePtr _publishImage;
    cv_bridge::CvImage _image;
    _image.encoding = "bgr8";


	if (argc >= 2)
	{
		ROS_INFO(argv[1]);
		cam.open(argv[1]);
		camOpen=true;
	}
	else
		ros::shutdown();


    ros::Rate _looprate(5);

    while(ros::ok())
    {
        if(camOpen)
        {
            ROS_INFO("publishing image on /kraken/bottom_camera\n");
            cam >> _image.image;

            _publishImage = _image.toImageMsg();

            _image_pub.publish(_publishImage);
        }
        else{
            ROS_INFO("video cannot be opened");
        }

        ros::spinOnce();
        _looprate.sleep();
    }

    return 0;
}
