/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: pranay
 *
 * Created on 8 May, 2016, 3:36 PM
 */

//#include "pstream.h"
#include "vw_detect.h"
using namespace std;
using namespace cv;



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
    medianBlur(denoised, denoised, 5);
    Mat prediction(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));
   

    vw_detect new_vw(argv[2],4);
    
    new_vw.getPredictions(denoised,prediction);
    new_vw.wait_for_completion();

    imshow("Original Image", image);
    imshow("Denoised Image", denoised);
    imshow("Prediction", prediction);
    moveWindow("Original Image", 100, 150);
    moveWindow("Denoised Image", 100, 150);
    moveWindow("Prediction", 100, 150);
    waitKey(0);

    return 0;
}

