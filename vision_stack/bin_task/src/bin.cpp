#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>

using namespace std;
using namespace cv;

Point2f computeIntersect(Vec4i a, Vec4i b)
{
    int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3];
    int x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];

    if (float d = ((float)(x1-x2) * (y3-y4)) - ((y1-y2) * (x3-x4)))
    {
        Point2f pt;
        pt.x = ((x1*y2 - y1*x2) * (x3-x4) - (x1-x2) * (x3*y4 - y3*x4)) / d;
        pt.y = ((x1*y2 - y1*x2) * (y3-y4) - (y1-y2) * (x3*y4 - y3*x4)) / d;
        return pt;
    }
    else
        return Point2f(-1, -1);
}

int main(int argc, char** argv)
{

    Mat orgimg, finalimg, hsv, thresh,edges;
    double slope;
    vector<Vec4i> _linesx, _linesy;

    Scalar lower(0,127,127), upper(255,255,255);
    orgimg = imread(argv[1],1);

    if(!orgimg.data)
    {
        cout << "no image loaded..\n";
        return 0;
    }
    Mat newimg(orgimg.rows, orgimg.cols, CV_8UC3, Scalar(0,0,255));
    vector<vector<Point> > contours;
    vector<vector<Point> > polygon;
    cvtColor(orgimg,hsv,CV_BGR2HSV);
    vector<Vec4i> lines;
    inRange(hsv,lower, upper,thresh);



    imshow("inrange thresh",thresh);
    if(!hsv.data && !thresh.data)
        return 0;


    findContours(thresh,contours,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE,Point(0,0));
    polygon.resize(contours.size());

    cout << contours.size() << " = contours size " << endl;
    for (int i = 0; i < contours.size(); ++i) {
        approxPolyDP(Mat(contours[i]),polygon[i],3,true);
        drawContours(thresh,polygon,i,Scalar(255,0,0),2);
    }


    ///! checking number of corners in the contour
    ///! number of corners  should be 12 ...

    cout << polygon.size() << endl;
    for (int i = 0; i < polygon.size(); ++i) {
        if(!isContourConvex(polygon[i]))
            cout << "polygon[" << i<<"] is not convex.." <<endl;
    }
    cout << "polygon 0 ka size = " << polygon[0].size() << endl;
    vector<Point2f> corners;


    Canny(thresh,thresh,20,200,3);
    HoughLinesP(thresh,lines,1,CV_PI/180,20,20,10);

    cout << lines.size() << " = number of lines " << endl;

    double m1, m2, angle;
    int count=0;
    for (int i = 0; i < lines.size() - 1; ++i) {
        //        line(newimg,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]),Scalar(255,255,0),1);
        if(lines[i][2]-lines[i][0] !=0){
            slope = atan((double)(lines[i][3]-lines[i][1])/(double)(lines[i][2]-lines[i][0]));
            cout << "slope for line[" << i<<"] = " << slope << endl;
            //            line(newimg,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]),Scalar(255,0,255),1);
        }
        else
            line(newimg,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]),Scalar(255,0,0),1);

        //        m1 = atan((double)(lines[i][3]-lines[i][1])/(double)(lines[i][2]-lines[i][0]));
        //        m2 = atan((double)(lines[i+1][3]-lines[i+1][1])/(double)(lines[i+1][2]-lines[i+1][0]));

        //        angle = m1-m2;
        //        cout << "angle = " << angle << endl;


        //        if(angle < 110 && angle > 70 ){
        //            count++;
        //            line(newimg,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]),Scalar(0,255,0),1);
        //        }

        //        cout <<"count = "<< count<< endl;
        //        if(slope<CV_PI/180*30 && slope>150*CV_PI/180){
        //            _linesx.push_back(lines[i]);
        //            cout << "slope of line drawn in x = " << slope << endl;
        //            line(x_lines,Point(_linesx[i][0],_linesx[i][1]),Point(_linesx[i][2],_linesx[i][3]),Scalar(255,255,0),3);
        //        }
        //        if(slope>CV_PI/180*60 && slope<CV_PI/180*120){
        //            _linesy.push_back(lines[i]);
        //            cout << "slope of line drawn in y = " << slope << endl;
        //            line(y_lines,Point(_linesy[i][0],_linesy[i][1]),Point(_linesy[i][2],_linesy[i][3]),Scalar(255,255,0),3);
        //        }

    }



    for (int i = 0; i < lines.size(); ++i) {
        int count = 0;
        if(lines[i][2]-lines[i][0] !=0){
            m1 = atan((double)(lines[i][3]-lines[i][1])/(double)(lines[i][2]-lines[i][0]));
            cout << "slope for line[" << i<<"] = " << slope << endl;
            line(newimg,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]),Scalar(255,0,255),1);
        }
        else{
            m1 = 90*CV_PI/180;
            line(newimg,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]),Scalar(255,0,0),1);
        }

//        m1 = atan((double)(lines[i][3]-lines[i][1])/(double)(lines[i][2]-lines[i][0]));


        angle = m1-m2;
        cout << "angle = " << angle << endl;
        for (int j = 0; j < lines.size(); ++j) {
               m2 = atan((double)(lines[j][3]-lines[j][1])/(double)(lines[j][2]-lines[j][0]));
               angle = m1- m2;
               if(angle <100*CV_PI/180 && angle>75*CV_PI/180){
                   count++;
                   cout << "count for line[" << i << "] = " << count << endl;
//                   line(newimg,)
               }
        }
    }
    ///! find intersection points of lines
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = i+1; j < lines.size(); j++)
        {
            Point2f pt = computeIntersect(lines[i], lines[j]);
            if (pt.x >= 0 && pt.y >= 0)
                corners.push_back(pt);
            circle(newimg,corners[i],5,Scalar(255,0,255),2);
        }
    }

    cout << "corners size = " << corners.size() << endl;







    imshow("win", newimg);

    //    cout << _linesx.size() << endl;
    //    cout << _linesy.size() << endl;

    //    cout << lines.size() << endl;
    //    imshow("x_lines", x_lines);
    //    imshow("y_lines", y_lines);
    imshow("orgimg", orgimg);
    imshow("thresh", thresh);
    waitKey(0);
    return 0;
}

