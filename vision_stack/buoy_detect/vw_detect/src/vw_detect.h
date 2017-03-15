/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   vw_test.h
 * Author: pranay
 *
 * Created on 24 May, 2016, 7:39 AM
 */

#ifndef VW_DETECT_H
#define VW_DETECT_H

#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <iostream>
#include <stdint.h>
#include "ctpl.h"
#include <vector>


using namespace std;

typedef vector<uint_least8_t> v1d;
typedef vector<v1d> v2d;
typedef vector<v2d> v3d;

class vw_detect
{
public:
    int n_threads;
    //OpenCV uses  H: 0 - 180, S: 0 - 255, V: 0 - 255 ;
    //http://stackoverflow.com/questions/10948589/choosing-correct-hsv-values-for-opencv-thresholding-with-inranges
    v3d hash_table = v3d(181, v2d(256, v1d(256)));
    void vw_detect_init(char*);
    void getPredictions(cv::Mat, cv::Mat&);
    bool is_idle();
    void clean_up();
    vw_detect(char *path_to_hash, int n = 4);
    ~vw_detect();
    void wait_for_completion();
    void predict_block(cv::Mat, cv::Mat&, int, int);
private:
    ctpl::thread_pool pool;

};

#endif /* VW_TEST_H */

