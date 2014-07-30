#include "ros/ros.h"
#include <iostream>
#include "premap/map.hpp"
#include "premap/getLocation.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "xml-parser/rapidxml-1.13/rapidxml.hpp"
#include "opencv/cv.h"
#include "resources/topicHeader.h"


using namespace rapidxml;
using namespace std;

using namespace premap_module;
using namespace ros;
using namespace std;
const char* ESTIMATED_POSE=topics::NAV_POSE_ESTIMATED,
*SERVICE_NAME=topics::PREMAP_LOCATION_SERVICE;

Point2f* returnArray (int &n,std::string file_name);
int main(int argc,char**argv)
{
    init(argc,argv,"map_node");
    if(argc<2)
    {
        cout<<argv[0]<<endl;
        cout<<argv[1]<<endl;
        ROS_ERROR("sry no argument given to load xml file");
        return 0;
    }
    NodeHandle n;
    int _nmarker;
    Point2f* prtm=returnArray (_nmarker,argv[1]);
    Map m(n,prtm,_nmarker);
    Subscriber pos_sub=n.subscribe(ESTIMATED_POSE,100,&Map::updateVehiclePosition,&m);
    ros::ServiceServer  _server(n.advertiseService(string(SERVICE_NAME),&Map::getObjectLocation,&m ));
    ros::spin();
    return 0;

}


////Parses the xml-file
//Point2f * returnArray(int &n){
//    int size = 0;

//    xml_document<> doc;
//    xml_node<> *root_node, *subNode;
//    ifstream theFile ("map_xml.xml");


//    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());




//    buffer.push_back('\0');
//    doc.parse<0>(&buffer[0]);

//    root_node = doc.first_node();

//    if(root_node==NULL)
//        ROS_INFO("theres no xml file loaded");
//    subNode = root_node->first_node();

//    for( ; subNode ; subNode = subNode->next_sibling(), ++size);
//    size=6;

//    Point2f * marker_array = new Point2f[size];

//    subNode = root_node->first_node();
//    string g;

//    for( int i = 0 ; subNode ; ++i, subNode = subNode->next_sibling()){

//        g = std::string(subNode->first_attribute("x")->value());
//        int x = atoi(g.c_str());
//        g = std::string(subNode->first_attribute("y")->value());
//        int y = atoi(g.c_str());
//        Point2f pt(x, y);
//        marker_array[i] = pt;
//    }
//    n = size;
//    return marker_array;
//}
