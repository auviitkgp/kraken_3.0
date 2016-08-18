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
#include "ctpl_stl.h"
//#include <sstream>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <unistd.h>
//#include <signal.h>
//

class vw_detect
{
public:
    int n_threads;
    uint_least8_t hash_table[256][256][256];
    void vw_detect_init(char*);
    void getPredictions(cv::Mat, cv::Mat);
    bool is_idle();
    void clean_up();
    vw_detect(char *path_to_hash, int n = 4);
    ~vw_detect();
    void wait_for_completion();
private:
    ctpl::thread_pool pool;
    void predict_block(cv::Mat, cv::Mat, int, int);

    /*
    static void write_to_input(cv::Mat, int in_handle);
    static void read_from_output(cv::Mat, int out_handle);

     */
};

#endif /* VW_TEST_H */

