#ifndef GROW_H
#define GROW_H
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

using namespace std;
using namespace cv;

class grow
{
public:
    grow(int colorThreshold = 20, int whiteThreshold = 10);
    void start_grow(Mat input, Mat filled, Mat edgeMap, int sX, int sY, int colorflag);
    void setThresholds(int colorThreshold, int whiteThreshold);
    //    ~grow();

private:
    int colorThreshold;
    int whiteThreshold;
};
#endif