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
    double r;
    color_enum color;
}
point;

Mat image;
Mat colour_detect;
vw_detect *detector;
ros::NodeHandle *nh;
image_transport::ImageTransport *it;
image_transport::Publisher *ml_pub, *final_pub;
ros::Publisher *result_pub;

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
    Mat filtered;
    medianBlur(image, filtered, 5);


    detector->getPredictions(filtered, colour_detect);
    //    temp1.deallocate();
    //    temp2.deallocate();

    detector->wait_for_completion();

    cv_ptr->image = colour_detect;
    ml_pub->publish(cv_ptr->toImageMsg());
    auto t2 = clk::now();
    //main code (vw detect + region growing)

    Mat eroded;
    erode(colour_detect, eroded, Mat(), Point(-1, -1), 1);

    Mat gray;
    cvtColor(eroded, gray, cv::COLOR_BGR2GRAY);

    vector<Vec3f> circles;
    HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 1, eroded.rows / 8, 200, 100, 0, 0);

    auto t3 = clk::now();

    vector<point> yellow_detected, red_detected, green_detected;
    for (size_t i = 0; i < circles.size(); i++)
    {
        point current;
        current.x = circles[i][0];
        current.y = circles[i][1];
        current.r = circles[i][2];
        current.color = getColor(current.x, current.y, eroded);

        switch (current.color)
        {
        case yellow:
            yellow_detected.push_back(current);
            break;
        case red:
            red_detected.push_back(current);
            break;
        case green:
            green_detected.push_back(current);
            break;
        default:
            ROS_ERROR("A wild 'weird' buoy appeared");
        }
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // draw the circle center
        circle(eroded, center, 3, Scalar(255, 255, 255), -1, 8, 0);
        // draw the circle outline
        circle(eroded, center, radius, Scalar(255, 255, 255), 3, 8, 0);
    }
    kraken_msgs::center_color center_color_object;

    if (yellow_detected.size() > 1)
    {
        double deciding_factor_max = 0, index = 0;
        for (size_t i = 0; i < yellow_detected.size(); i++)
        {
            if (yellow_detected[i].r * yellow_detected[i].y > deciding_factor_max)
            {
                index = i;
                deciding_factor_max = yellow_detected[i].r * yellow_detected[i].y;
            }
        }
        center_color_object.yellow = 1;
        center_color_object.Y_x = yellow_detected[index].x;
        center_color_object.Y_y = yellow_detected[index].y;
        center_color_object.Y_radius = yellow_detected[index].r;
    }
    else if (yellow_detected.size() == 1)
    {
        center_color_object.yellow = 1;
        center_color_object.Y_x = yellow_detected[0].x;
        center_color_object.Y_y = yellow_detected[0].y;
        center_color_object.Y_radius = yellow_detected[0].r;
    }
    else
        center_color_object.yellow = 0;

    if (red_detected.size() > 1)
    {
        double deciding_factor_max = 0, index = 0;
        for (size_t i = 0; i < red_detected.size(); i++)
        {
            if (red_detected[i].r * red_detected[i].y > deciding_factor_max)
            {
                index = i;
                deciding_factor_max = red_detected[i].r * red_detected[i].y;
            }
        }
        center_color_object.red = 1;
        center_color_object.R_x = red_detected[index].x;
        center_color_object.R_y = red_detected[index].y;
        center_color_object.R_radius = red_detected[index].r;
    }
    else if (red_detected.size() == 1)
    {
        center_color_object.red = 1;
        center_color_object.R_x = red_detected[0].x;
        center_color_object.R_y = red_detected[0].y;
        center_color_object.R_radius = red_detected[0].r;
    }
    else
        center_color_object.red = 0;

    if (green_detected.size() > 1)
    {
        double deciding_factor_max = 0, index = 0;
        for (size_t i = 0; i < green_detected.size(); i++)
        {
            if (green_detected[i].r * green_detected[i].y > deciding_factor_max)
            {
                index = i;
                deciding_factor_max = green_detected[i].r * green_detected[i].y;
            }
        }
        center_color_object.green = 1;
        center_color_object.G_x = green_detected[index].x;
        center_color_object.G_y = green_detected[index].y;
        center_color_object.G_radius = green_detected[index].r;
    }
    else if (green_detected.size() == 1)
    {
        center_color_object.green = 1;
        center_color_object.G_x = green_detected[0].x;
        center_color_object.G_y = green_detected[0].y;
        center_color_object.G_radius = green_detected[0].r;
    }
    else
        center_color_object.green = 0;
    auto t4 = clk::now();
    result_pub->publish(center_color_object);
    cv_ptr->image = eroded;
    final_pub->publish(cv_ptr->toImageMsg());
    ROS_DEBUG("time to publish vw prediction : %d\n", (int) duration_cast<milliseconds>(t2 - t1).count());
    ROS_DEBUG("time waited in queue :  %d\n", (int) duration_cast<milliseconds>(t3 - t2).count());
    ROS_DEBUG("time for generating final image :  %d\n", (int) duration_cast<milliseconds>(t4 - t3).count());

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

    ros::Publisher result = nh->advertise<kraken_msgs::center_color> (topics::CAMERA_CENTER_COLOR, 1);
    image_transport::Publisher ml_image_pub = it->advertise(topics::CAMERA_FRONT_ML_IMAGE, 1);
    image_transport::Publisher final_image_pub = it->advertise(topics::CAMERA_FRONT_FINAL_IMAGE, 1);
    ml_pub = &ml_image_pub;
    final_pub = &final_image_pub;
    result_pub = &result;
    ros::Rate loop_rate(10);

    while (nodeHandle.ok())
    {
        ros::spinOnce();
        loop_rate.sleep();
    }
    nodeHandle.shutdown();
    return 0;
}



