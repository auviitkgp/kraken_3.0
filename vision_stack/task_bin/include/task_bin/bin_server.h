#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <actionlib/server/simple_action_server.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <actionmsg/binAction.h>

using namespace std;
using namespace cv;

typedef actionlib::SimpleActionServer<actionmsg::binAction> Server;

class Bin
{
private:
	ros::NodeHandle _n;
	Server _s;
	image_transport::ImageTransport _it;
	image_transport::Subscriber _sub;
	image_transport::Publisher _pub;
	Mat _image, templ;
	actionmsg::binFeedback _feedback;
	actionmsg::binResult _result;
	std::string _actionName;
	Point templ_center;

public:
	Bin(string name, int t);
	void executeCB(const actionmsg::binGoalConstPtr &_goal);
	void imageCallBack(const sensor_msgs::ImageConstPtr &msg);
	Point tempMatch(Point *X);
	~Bin();
};
