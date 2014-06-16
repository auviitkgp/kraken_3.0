#include "premap/map.hpp"
using namespace premap_module;


Map::Map(NodeHandle &node,Point2f* ptr,int nsize):
    _node_handle(node),MARKERS(ptr),_nMarkers(nsize)
{
    memset(_map_data,UNKNOWN,sizeof(_map_data));
    initMap ();
}
void Map::initMap()
{
    showMap ();
}

void Map::updateVehiclePosition(const kraken_msgs::krakenPoseConstPtr &est_pose_msg)
{
    updateVehiclePosition (Point3f(est_pose_msg->data[kraken_core::_px],est_pose_msg->data[kraken_core::_py],
                                   est_pose_msg->data[kraken_core::_py]));
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

void Map::updateVehiclePosition(const Point3f &new_position)
{
    _vehicle_Pos.position.x=roundOff (new_position.x);
    _vehicle_Pos.position.y=roundOff (new_position.y);
    _vehicle_Pos.position.z=roundOff(new_position.z);
    Point3i p(new_position.x*10,new_position.y*10,new_position.z);

    //set the current position as travelled
    _map_data[p.x][p.y][p.z]=TRAVELLED;



    for (int depth = 1,z; depth< CAM_DEPTH_MAX*10; ++z) {
        z=p.z-depth;
        float radius=depth*sin(CAMERA_ANGLE/2);

        for (int x = p.x-radius; x < p.x+radius ; ++x) {
            for (int y = p.y-radius; y < p.y+radius; ++y) {
                if(cv::norm(cv::Mat(Point(x,y)),cv::Mat(Point(p.x,p.y))) <= radius )
                    _map_data[x][y][z]=SEEN;
            }
        }
    }

    Point p2(p.x,p.y);
    updateMapVideo (p2);
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
    _image=Mat::ones(Size(700,700),CV_8UC3);
    cv::namedWindow ("win");

    for(int i=0; i<_nMarkers; i++)
    {
        circle(_image,Point(MARKERS[i].x*10,MARKERS[i].y*10),5,Scalar(255,0,0));
    }

    imshow("win",_image);
}

void Map::updateMapVideo(Point &position)
{
    Mat disp;
    disp=_image.clone ();
    circle(disp,position,5,Scalar(0,255,0));
    imshow ("win",disp);
    waitKey (40);

}





