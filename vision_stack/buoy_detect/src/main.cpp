/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: pranay
 *
 * Created on August 8, 2016, 4:18 AM
 */

#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vw_detect.h>
#include <grow.h>
using namespace std;
using namespace cv;

/*
 * 
 */
int main(int argc, char** argv)
{
    if (argc != 3)
    {
        cout << " Usage: display_image ImageToLoadAndDisplay" << endl;
        return -1;
    }

    Mat image;
    Mat denoised;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR); // Read the file
    //    fastNlMeansDenoisingColored(image, denoised, 2);
    medianBlur(denoised, denoised, 3);
    Mat prediction(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));


    vw_detect new_vw(argv[2], 4);
    new_vw.getPredictions(denoised, prediction);
    Mat contour_image;
    prediction.copyTo(contour_image);
    new_vw.wait_for_completion();
    vector< vector <Point> > contours;
    //    vector<Vec4i> hierarchy;
    vector<Point> seed_points;
    findContours(contour_image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    CvMoments moments;
    for (size_t i = 0; i < contours.size(); i++)
    {
        cvMoments(contours[i], &moments);
        //calculating the centers of the contours
        seed_points.push_back(Point((int)
                                    (cvGetSpatialMoment(&moments, 1, 0) / cvGetSpatialMoment(&moments, 0, 0)),
                                    cvGetSpatialMoment(&moments, 0, 1) / cvGetSpatialMoment(&moments, 0, 0)));
    }
   
    return 0;
}

