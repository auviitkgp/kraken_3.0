#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <std_msgs/String.h>
#include <kraken_msgs/center_color.h>

#include <resources/topicHeader.h>

#include <vw_detect.h>
#include <grow.h>

#include <chrono>
using namespace std;
using namespace cv;
using namespace std::chrono;
typedef std::chrono::high_resolution_clock clk;

enum color_enum
{
    black = 1, yellow = 2, green = 3, red = 4, other = 5
};

typedef struct point_
{
    int x, y;
    color_enum color;
}
point;

Mat image;
Mat colour_detect;
vw_detect *detector;
ros::NodeHandle *nh;
image_transport::ImageTransport *it;
image_transport::Publisher *ml_pub, *final_pub;

color_enum getColor(int x, int y, Mat image)
{
    Vec3b &Color = image.at<Vec3b>(x, y);

    if (Color[0] == 0 && Color[1] == 0 && Color[2] == 0)
        return black;
    if (Color[0] == 0 && Color[1] == 255 && Color[2] == 255) // Yellow pixel
        return yellow;

    if (Color[0] == 0 && Color[1] == 255 && Color[2] == 0) // GREEN pixel
        return green;

    if (Color[0] == 0 && Color[1] == 0 && Color[2] == 255) // RED pixel
        return red;
    return other;
}

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    
//    Mat temp1, temp2;
    cv_bridge::CvImagePtr cv_ptr;
    auto t1 = clk::now();
    try
    {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        /*
        temp1 = image[14];
        temp2 = colour_detect[14];
        for (int i = 0; i <= 13; i++)
        {
            image[i + 1] = image[i];
            colour_detect[i + 1] = colour_detect[i];
        }*/
        image = cv_ptr->image;
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    Mat newMat(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));
    colour_detect = newMat;
    // filters to be used before detection is done
    medianBlur(image, colour_detect, 5);


    detector->getPredictions(image, colour_detect);
//    temp1.deallocate();
//    temp2.deallocate();
    auto t2 = clk::now();
    detector->wait_for_completion();
    
    cv_ptr->image = colour_detect;
    ml_pub->publish(cv_ptr->toImageMsg());
    auto t3 = clk::now();
    //main code (vw detect + region growing)

    Mat eroded;
    erode(colour_detect, eroded, Mat(), Point(-1, -1), 2);
    
    Mat gray;
    cvtColor(eroded,gray,cv::COLOR_BGR2GRAY);

    vector < vector <Point> > contours;
    findContours(gray, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    
    vector<point> seed_points;
    for (size_t i = 0; i < contours.size(); i++)
    {
        Moments mu;
        mu = moments(contours[i]);
        point current;
        //calculating the centers of the contours
        current.x = mu.m10 / mu.m00; //(cvGetSpatialMoment(&moments, 1, 0) / cvGetSpatialMoment(&moments, 0, 0));
        current.y = mu.m01 / mu.m00; //(cvGetSpatialMoment(&moments, 0, 1) / cvGetSpatialMoment(&moments, 0, 0));
        current.color = getColor(current.x, current.y, eroded);
        seed_points.push_back(current);
    }
    Mat final_image(eroded.rows, eroded.cols, CV_8UC3), edgeMap(eroded.rows, eroded.cols, CV_8UC3);
    eroded.copyTo(final_image);
    grow growhandle;
    growhandle.setThresholds(15, 15);
    for (size_t i = 0; i < seed_points.size(); i++)
    {
        growhandle.start_grow(eroded, final_image, edgeMap, seed_points[i].x, seed_points[i].y, seed_points[i].color);
    }
    
    
    cv_ptr->image = final_image;
    final_pub->publish(cv_ptr->toImageMsg());
    auto t4 = clk::now();
    cout << "time re receive and queue : " << duration_cast<milliseconds>(t2 - t1).count() <<endl;
    cout << "time waited in queue : " << duration_cast<milliseconds>(t3 - t2).count() << endl;
    cout << "time for generating final image : " << duration_cast<milliseconds>(t4 - t3).count() << endl;
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        ROS_ERROR("Requires the path to vw hash");
    }
    ros::init(argc, argv, "buoy_detect_integrator");
    ros::NodeHandle nodeHandle;
    image_transport::ImageTransport imageTransport(nodeHandle);
    nh = &nodeHandle;
    it = &imageTransport;
    vw_detect detector1(argv[1]);
    detector = &detector1;

    image_transport::Subscriber sub = it->subscribe(topics::CAMERA_FRONT_RAW_IMAGE, 1, imageCallback);
    kraken_msgs::center_color center_color_object;
    ros::Publisher result = nh->advertise<kraken_msgs::center_color> (topics::CAMERA_CENTER_COLOR, 1);
    image_transport::Publisher ml_image_pub = it->advertise(topics::CAMERA_FRONT_ML_IMAGE, 1);
    image_transport::Publisher final_image_pub = it->advertise(topics::CAMERA_FRONT_FINAL_IMAGE, 1);
    ml_pub = &ml_image_pub;
    final_pub = &final_image_pub;

    ros::Rate loop_rate(10);

    while (nodeHandle.ok())
    {
        ros::spinOnce();
        loop_rate.sleep();
    }
    nodeHandle.shutdown();
    return 0;
}



