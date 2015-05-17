#include <ros/ros.h>
#include <task_bin/bin_server.h>
#include <resources/topicHeader.h>

Bin::Bin(std::string name, int t=0): _it(_n), _s(_n, name, boost::bind(&Bin::executeCB, this, _1), false), _actionName(name)
{
	_sub = _it.subscribe(topics::CAMERA_FRONT_RAW_IMAGE, 1, &Bin::imageCallBack, this);
  _pub = _it.advertise("/kraken/temp/", 1); // TODO: Create a topic in resources.
  switch(t)
  {
    case 0:
    {
      templ = imread("RoboSub2015_silhouettes1.png");
      break;
    }
    case 1:
    {
      templ = imread("RoboSub2015_silhouettes2.png");
      break;
    }
    case 2:
    {
      templ = imread("RoboSub2015_silhouettes3.png");
      break;
    }
    case 3:
    {
      templ = imread("RoboSub2015_silhouettes4.png");
      break;
    }
  }
  _s.start();
}

void Bin::executeCB(const actionmsg::binGoalConstPtr &_goal)
{
	ros::Rate looprate(10);
  bool _detected;
  Point templ_center;

  switch(_goal->order)
  {
    case DETECT_SILHOUETTE:
    {
     while(ros::ok())
     {
      if (_s.isPreemptRequested() || !ros::ok())
      {
        ROS_INFO("%s: Preempted", _actionName.c_str());
        _s.setPreempted();
        break;
      }
      _detected = tempMatch(&templ_center);
      looprate.sleep();
    }
    break;
  }
}
}

void Bin::imageCallBack(const sensor_msgs::ImageConstPtr &_msg)
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

bool tempMatch(Point *X)
{
  bool match;
  if(!_image.empty())
  {
    Mat img, result;
    feed_image.copyTo( img );

    int result_cols =  img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;

    result.create( result_rows, result_cols, CV_32FC1 );

    //Here, we are using SQDIFF_NORMED matching method

    matchTemplate( img, templ, result, 1 );
    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

    double minVal; double maxVal; Point minLoc; Point maxLoc; Point matchLoc;

    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

    if(minVal < 20)
      match = true;
    else
      match = false;
    matchLoc = minLoc;
    *X = Point(matchLoc.x + (templ.cols)/2 , matchLoc.y + (templ.rows)/2 );

    rectangle( img, matchLoc, Point( matchLoc.x + templ1.cols , matchLoc.y + templ1.rows ), Scalar::all(0), 2, 8, 0 );
    rectangle( result, matchLoc, Point( matchLoc.x + templ1.cols , matchLoc.y + templ1.rows ), Scalar::all(0), 2, 8, 0 );

    namedWindow( image_window, CV_WINDOW_AUTOSIZE );
    namedWindow( result_window, CV_WINDOW_AUTOSIZE );

    imshow( image_window, img );
    imshow( result_window, result );

    return match;
  }
  else
    return false;
}

int main(int argc, char ** argv)
{
  int t;
  ros::init(argc, argv, "bin_server");
  if (argc >= 2)
    t = atoi(argv[1]);
  else
    ros::shutdown();

  Bin _binserver("bin", 0);
  ros::spin();
  return 0;
}