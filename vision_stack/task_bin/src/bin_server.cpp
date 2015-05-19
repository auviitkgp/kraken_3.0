#include <ros/ros.h>
#include <ros/package.h>
#include <task_bin/bin_server.h>
#include <resources/topicHeader.h>

char* const image_window = "Source Image";
char* const result_window = "Result window";

Bin::Bin(std::string name, int t): _it(_n), _s(_n, name, boost::bind(&Bin::executeCB, this, _1), false), _actionName(name)
{
	_sub = _it.subscribe(topics::CAMERA_BOTTOM_RAW_IMAGE, 1, &Bin::imageCallBack, this);
  	_pub = _it.advertise(topics::CAMERA_BOTTOM_BIN_IMAGE, 1);
  	DIR = ros::package::getPath("task_bin");
  	switch(t)
  	{
    	case 0:
    	{
    		path = DIR + "/RoboSub2015_silhouettes1.png";
      		templ = imread(path);
      		break;
    	}
    	case 1:
    	{
    		path = DIR + "/RoboSub2015_silhouettes2.png";
      		templ = imread(path);
      		break;
    	}
    	case 2:
    	{
    		path = DIR + "/RoboSub2015_silhouettes3.png";
      		templ = imread(path);
      		break;
    	}
    	case 3:
    	{
    		path = DIR + "/RoboSub2015_silhouettes4.png";
      		templ = imread(path);
      		break;
    	}
  	}
  	_s.start();
}

void Bin::executeCB(const actionmsg::binGoalConstPtr &_goal)
{
	ros::Rate looprate(10);
  bool _detected;

  switch(_goal->order)
  {
    case 0:
    {
     while(ros::ok())
     {
      if (_s.isPreemptRequested() || !ros::ok())
      {
        ROS_INFO("%s: Preempted", _actionName.c_str());
        _s.setPreempted();
        break;
      }
      _detected = tempMatch();
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

bool Bin::tempMatch()
{
  bool match;
  if(!_image.empty())
  {
  	double minVal; double maxVal; Point minLoc; Point maxLoc; Point matchLoc;
  	//minMaxLoc(_image, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
  	//_image.convertTo(_image, CV_32FC1);
  	//templ.convertTo(templ, CV_32FC3);
  	//cvtColor(templ, templ, CV_BGR2GRAY);
    Mat img, result, tmp;
    
    _image.copyTo( img );
    
    int result_cols =  img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;

    result.create( result_rows, result_cols, CV_32FC1 );

    //Here, we are using SQDIFF_NORMED matching method
    
    matchTemplate( img, templ, result, 1 );
    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

    if(minVal < 20) // TO BE CHECKED
      match = true;
    else
      match = false;
    matchLoc = minLoc;
    templ_center = Point(matchLoc.x + (templ.cols)/2 , matchLoc.y + (templ.rows)/2 );

    rectangle( img, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
    rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );

    namedWindow( image_window, CV_WINDOW_AUTOSIZE );
    namedWindow( result_window, CV_WINDOW_AUTOSIZE );

    imshow( image_window, img );
    imshow( result_window, result );

    waitKey(0);

    return match;
  }
  else
  {
  	ROS_INFO("NO IMAGE LOADED\n");
   	return false;
  }
}

int main(int argc, char ** argv)
{
  int t;
  ros::init(argc, argv, "bin_server");
  if (argc >= 2)
    t = atoi(argv[1]);
  else
  {
    ROS_INFO("An integer 0, 1, 2 or 3  must be passed as argument to get the template to be matched.\n");
    ros::shutdown();
  }

  Bin _binserver("bin", t);
  ros::spin();
  return 0;
}