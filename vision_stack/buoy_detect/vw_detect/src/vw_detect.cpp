/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   vw_test.cpp`
 * Author: pranay
 *
 * Created on 8 May, 2016, 3:36 PM
 */
#include "vw_detect.h"
#include <fstream>
#include <fcntl.h>
#include <iostream>
#include <stdint.h>
#include "ctpl_stl.h"
#include <unistd.h>

using namespace std;
using namespace cv;

void vw_detect::vw_detect_init(char *path_to_hash)
{
    /*
    ifstream in_file(path_to_hash);
    if (in_file.is_open())
    {
        
    }
     */
    char buff[256];
    int file_handle = open(path_to_hash, O_RDONLY);
    if (file_handle == -1)
    {
        cerr << "Cannot open file";
        return;
    }
    for (int h = 0; h < 256; h++)
    {
        for (int s = 0; s < 256; s++)
        {
            if (read(file_handle, buff, sizeof (buff)) != 256)
            {
                cerr << "error reading file at " << h << " " << s << "\n";
                return;
            }
            for (int v = 0; v < 256; v++)
            {
                hash_table[h][s][v] = (uint_least8_t) (buff[v] - '0');
            }
        }
    }
    ctpl::thread_pool pool(n_threads);

}

void vw_detect::getPredictions(Mat original, Mat prediction)
{
    Mat hsv_image;
    cvtColor(original, hsv_image, CV_BGR2HSV);
    //    prediction = Mat(original.rows, original.cols, CV_8UC3, Scalar(0, 0, 0));
    for (int i = 0; i <= 255; i += original.rows / n_threads)
    {
        //pool.push(vw_detect::predict_block, hsv_image, prediction, i, i + original.rows / n_threads - 1,this->hash_table);
        pool.push([&, this](int id)
        {
            this->predict_block(hsv_image, prediction, i, i + original.rows / n_threads - 1);
        });
    }

}

bool vw_detect::is_idle()
{
    return (pool.n_idle() == pool.size());
}

void vw_detect::predict_block(Mat hsv_image, Mat prediction, int start, int end)
{
    for (int i = start; i <= end; i++)
    {
        for (int j = 0; j < hsv_image.cols; j++)
        {
            Vec3b &hsv = hsv_image.at<Vec3b>(i, j);

            Vec3b &Color = prediction.at<Vec3b>(i, j);
            switch (hash_table[hsv[0]][hsv[1]][hsv[2]])
            {
            case 1:
                Color[0] = 0;
                Color[1] = 0;
                Color[2] = 0;
                break;

            case 2:
                Color[0] = 0;
                Color[1] = 255;
                Color[2] = 255;
                break;

            case 3:
                Color[0] = 0;
                Color[1] = 255;
                Color[2] = 0;
                break;

            case 4:
                Color[0] = 0;
                Color[1] = 0;
                Color[2] = 255;
                break;
            default:
                cerr << "Unknown prediction at (" << i << "," << j << ")." << endl;
                return;
            }

        }
    }
}

void vw_detect::clean_up()
{
    pool.stop(0);
}

vw_detect::vw_detect(char *path_to_hash, int n) : n_threads(n)
{
    vw_detect_init(path_to_hash);
}

vw_detect::~vw_detect()
{
    vw_detect::clean_up();
}

void vw_detect::wait_for_completion()
{
    if (!is_idle())
        usleep(2400);
}

