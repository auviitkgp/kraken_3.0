#include "bottomcam/bottomcam.h"

Camera camObj;

void msgCallback(const std_msgs::String::ConstPtr& msg)
{
    int camMsg = atoi(msg->data.c_str());

    if (camMsg == camObj.getCurrentCameraNo() && !camObj.isOpen())
    {
        if (camObj.returnCamera().open(camMsg))
        {
            std::cout << "Camera " << camMsg << " opened successfully.";
            camObj.setCameraNo(camMsg);
        }
        else
        {
            std::cout << "Unable to open camera" << "\n";
            std::cout << "Exiting";
            ros::shutdown();
        }
    }
    else
        if (camMsg != camObj.getCurrentCameraNo() && camObj.isOpen())
        {
            camObj.returnCamera().release();
            std::cout << "Camera " << camObj.getCurrentCameraNo() << " closed successfully";
            camObj.setCameraState(false);
        }
}

int main(int argc, char** argv)
{

    camObj.inspect();

    ros::init(argc, argv, "bottomcam");

    ros::NodeHandle _nh;
    image_transport::ImageTransport _it(_nh);
    image_transport::Publisher _image_pub = _it.advertise(topics::CAMERA_BOTTOM_RAW_IMAGE, 1);
    ros::Subscriber _sub = _nh.subscribe(topics::CAMERA_CAM_SWITCH, 1, msgCallback);

    sensor_msgs::ImagePtr _publishImage;
    cv_bridge::CvImage _image;
    _image.encoding = "bgr8";

    cv::VideoCapture _this_cam = camObj.returnCamera();
    cv::VideoCapture cam;

    if (argc >= 2)
    {
        int _cameraNo = atoi(argv[1]);
        camObj.setCameraNo(_cameraNo);

        if (!_this_cam.open(_cameraNo))
        {
            std::cout << "Not able to open camera" << "\n";
            std::cout << "Exiting";
            ros::shutdown();
        }

        camObj.setCameraState(true);
    }
    else
    {
        std::cout << "Atleast 1 argument is required.";
        std::cout << "0 for bottom camera, 1 for front camera";
        ros::shutdown();
    }


    double temp_rate;
//  std::string string_rate;
    if (_nh.hasParam("/ros_rate"))
    {
        _nh.getParam("/ros_rate", temp_rate);
//      temp_rate = atof(string_rate.c_str());
    }
    else
    {
        std::runtime_error::runtime_error("ROSParam '/ros_rate' does not exist.");
        ROS_ERROR("ROSParam '/ros_rate' does not exist.");
        ros::shutdown();
        return ;

    }
    ros::Rate _looprate(temp_rate);

    while(ros::ok())
    {
        if (camObj.isOpen())
        {
            _this_cam >> _image.image;

            // cv::imshow(_image.image);

            _publishImage = _image.toImageMsg();

            _image_pub.publish(_publishImage);
        }

        ros::spinOnce();
        _looprate.sleep();
    }

    return 0;
}
