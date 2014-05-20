#include "premap/map.hpp"
#include "premap/getLocation.h"
#include "geometry_msgs/Pose.h"
#include "cstdio"
#include "cstring"
#include "opencv2/highgui/highgui.hpp"
Map::Map(NodeHandle &node,Point2f* ptr,int nsize):
    _n(node),MARKERS(ptr),_nMarkers(nsize)
{
    initMap ();
}
void Map::initMap()
{
    showMap ();

}

void Map::updateVehiclePosition(const geometry_msgs::PoseConstPtr& pos_msg)
{
    updateVehiclePosition (Point2f(pos_msg->position.x,pos_msg->position.y));
}

bool Map::getObjectLocation(premap::getLocation::Request &req, premap::getLocation::Response &res)
{
    int _id=req.id;
    if(_id>=_nMarkers||_id<0)
        return false;
    res.x=MARKERS[_id].x;
    res.y=MARKERS[_id].y;
    return true;
}

void Map::updateVehiclePosition(const Point2f &new_position)
{
    _vehicle_Pos.x=roundOff (new_position.x);
    _vehicle_Pos.y=roundOff (new_position.y);
    Point p(new_position.x*10,new_position.y*10);
    updateMapVideo (p);
}

float Map::roundOff(float f)
{
    char _a[10];
    sprintf (_a,"%.1f",f);
    return atof (_a);
}

void Map::showMap()
{
    vector<Point2f> center;
    image=Mat::ones(Size(700,700),CV_8UC3);
    cv::namedWindow ("win");

    for(int i=0; i<_nMarkers; i++)
    {
        circle(image,Point(MARKERS[i].x*10,MARKERS[i].y*10),5,Scalar(255,0,0));
    }

    imshow("win",image);
}

void Map::updateMapVideo(Point &position)
{
    Mat disp;
    disp=image.clone ();
    circle(disp,position,5,Scalar(0,255,0));
    imshow ("win",disp);
    waitKey (40);

}





