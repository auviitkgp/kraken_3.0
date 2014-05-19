#ifndef MAP_PARSE_h
#define MAP_PARSE_h
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "rapidxml-1.13/rapidxml.hpp"

#include "opencv/cv.h"

using namespace rapidxml;
using namespace std;
using namespace cv;

Point2f * returnArray(int &n){
    int size = 0;

    xml_document<> doc;
    xml_node<> *root_node, *subNode, *sub1Node;
    ifstream theFile ("map_xml.xml");
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);

    root_node = doc.first_node();

//    cout << "\n The first node is :- " << root_node->name() << "\n";

    subNode = root_node->first_node();

    for( ; subNode ; subNode = subNode->next_sibling(), ++size);

    Point2f * marker_array = new Point2f[size];

    subNode = root_node->first_node();
    string g;

    for( int i = 0 ; subNode ; ++i, subNode = subNode->next_sibling()){

        g = std::string(subNode->first_attribute("x")->value());
        int x = atoi(g.c_str());
        g = std::string(subNode->first_attribute("y")->value());
        int y = atoi(g.c_str());
        Point2f pt(x, y);
        marker_array[i] = pt;
    }

//    cout << "The size of the array returned is :- " << size << "\n";

//    cout << "The points are :- \n";

//    for(int j = 0; j < size; ++j)

//        cout << marker_array[j].x << ", " << marker_array[j].y << "\n";

    n = size;

    return marker_array;
}

int main()
{

    int size;
    Point2f * printer = returnArray(size);

    cout << size << "\n";

    for(int j = 0; j < size; ++j)

        cout << printer[j].x << ", " << printer[j].y << "\n";
}
#endif
