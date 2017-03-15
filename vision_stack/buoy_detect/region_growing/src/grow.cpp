#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <sstream>
#include <list>
#include "Color.hpp"
#include "grow.h"

using namespace std;
using namespace cv;

/*
        Function to determine the color difference between two pixel points.
        This function also calculates the color distance between white pixel
        and input pixel to deal overexposed regions of buoy.
        This function implements CIEDE2000 algorithm to find the distance in color
 */
bool colorDistance(Vec3b a, Vec3b b, int colorThreshold, int whiteThreshold)
{
    std::vector<double> ca, cb, cw(3, 255);

    ca.push_back((double) a[2]);
    ca.push_back((double) a[1]);
    ca.push_back((double) a[0]);

    cb.push_back((double) b[2]);
    cb.push_back((double) b[1]);
    cb.push_back((double) b[0]);

    float dist1 = kallaballa::ciede2000_distance(ca, cb);
    float dist2 = kallaballa::ciede2000_distance(cb, cw);

    if ((int) dist1 <= colorThreshold || (int) dist2 <= whiteThreshold)
        return true;
    else
        return false;
}

/*
        Function to modify pixel values at a point as per seed pixel
 */
void modifyPixel(Mat input, int x, int y, int colorflag)
{
    Vec3b &colorC = input.at<Vec3b>(x, y);

    //Yellow Pixel
    if (colorflag == 2)
    {
        colorC[0] = 0;
        colorC[1] = 255;
        colorC[2] = 255;
    }

    //Green Pixel
    if (colorflag == 3)
    {
        colorC[0] = 0;
        colorC[1] = 255;
        colorC[2] = 0;
    }

    //Red Pixel
    if (colorflag == 4)
    {
        colorC[0] = 0;
        colorC[1] = 0;
        colorC[2] = 255;
    }
}

/*
        Function to encode each pixel based on x and y	
 */
string intToString(int x, int y)
{
    string s = "";
    stringstream ss;

    ss << x;
    s += ss.str() + "-";
    ss.str("");
    ss << y;
    s += ss.str();

    return s;
}

/*
        Constructor sets default values to colorThreshold and whiteThreshold
 */
grow::grow(int colorThreshold, int whiteThreshold) : colorThreshold(colorThreshold), whiteThreshold(whiteThreshold)
{
    //	this->colorThreshold = 20;
    //	this->whiteThreshold = 10;
}

/*
        Region Growing algorithm, which is flood type algorithm
        filled -> output image with filled buoys
        edgeMap -> output image with only edges of final blobs
        sX --> Seed Pixel x value
        sY --> Seed Pixel y value
        colorflag --> to determine the color to be filled with
 */
void grow::start_grow(Mat input, Mat filled, Mat edgeMap, int sX, int sY, int colorflag)
{
    int x, y;
    long int count = 1;
    String s = "";
    Vec3b seed = input.at<Vec3b>(sX, sY);
    vector < vector<bool> > reach(input.rows, vector<bool>(input.cols, false));

    list <string> queue;

    reach[sX][sY] = true;

    modifyPixel(filled, sX, sY, colorflag);

    s = intToString(sX, sY);
    queue.push_back(s);

    while (!queue.empty())
    {
        s = queue.front();
        queue.pop_front();

        std::size_t pos = s.find('-');
        stringstream ssx(s.substr(0, pos));
        stringstream ssy(s.substr(pos + 1));
        ssx >> x;
        ssy >> y;

        //Right Pixel
        if (x + 1 < input.rows && (!reach[x + 1][y]))
        {
            if (colorDistance(seed, input.at<Vec3b>(x + 1, y), colorThreshold, whiteThreshold))
            {
                //cout << "reached right pixel" << endl;
                reach[x + 1][y] = true;
                s = intToString(x + 1, y);
                queue.push_back(s);
                //updateMean(seed, input.at<Vec3b>(x + 1, y), count);
                modifyPixel(filled, x + 1, y, colorflag);
                ++count;
            }
            else
                modifyPixel(edgeMap, x + 1, y, colorflag);

        }

        //Below Pixel
        if (y + 1 < input.cols && (!reach[x][y + 1]))
        {
            if (colorDistance(seed, input.at<Vec3b>(x, y + 1), colorThreshold, whiteThreshold))
            {
                //cout << "reached Below pixel" << endl;
                reach[x][y + 1] = true;
                s = intToString(x, y + 1);
                queue.push_back(s);
                //updateMean(seed, input.at<Vec3b>(x, y + 1), count);
                modifyPixel(filled, x, y + 1, colorflag);
                ++count;
            }
            else
                modifyPixel(edgeMap, x, y + 1, colorflag);
        }

        //cout << "+++seed Pixel" << endl;
        //cout << "seed[0] : " << (int)seed[0] << ", seed[1] : " << (int)seed[1] << ", seed[2] = " << (int)seed[2] << endl;

        //Left Pixel
        if (x - 1 >= 0 && (!reach[x - 1][y]))
        {
            if (colorDistance(seed, input.at<Vec3b>(x - 1, y), colorThreshold, whiteThreshold))
            {
                //cout << "reached left pixel" << endl;
                reach[x - 1][y] = true;
                s = intToString(x - 1, y);
                queue.push_back(s);
                //updateMean(seed, input.at<Vec3b>(x - 1, y), count);
                modifyPixel(filled, x - 1, y, colorflag);
                ++count;
            }
            else
                modifyPixel(edgeMap, x - 1, y, colorflag);
        }

        //Above Pixel
        if (y - 1 >= 0 && (!reach[x][y - 1]))
        {
            if (colorDistance(seed, input.at<Vec3b>(x, y - 1), colorThreshold, whiteThreshold))
            {
                //cout << "reached Above pixel" << endl;
                reach[x][y - 1] = true;
                s = intToString(x, y - 1);
                queue.push_back(s);
                //updateMean(seed, input.at<Vec3b>(x, y - 1), count);
                modifyPixel(filled, x, y - 1, colorflag);
                ++count;
            }
            else
                modifyPixel(edgeMap, x, y - 1, colorflag);
        }

        //Bottom Right Pixel
        if (x + 1 < input.rows && y + 1 < input.cols && (!reach[x + 1][y + 1]))
        {
            if (colorDistance(seed, input.at<Vec3b>(x + 1, y + 1), colorThreshold, whiteThreshold))
            {
                //cout << "reached Bottom Right pixel" << endl;
                reach[x + 1][y + 1] = true;
                s = intToString(x + 1, y + 1);
                queue.push_back(s);
                //updateMean(seed, input.at<Vec3b>(x + 1, y + 1), count);
                modifyPixel(filled, x + 1, y + 1, colorflag);
                ++count;
            }
            else
                modifyPixel(edgeMap, x + 1, y + 1, colorflag);
        }

        //Upper Right Pixel
        if (x + 1 < input.rows && y - 1 >= 0 && (!reach[x + 1][y - 1]))
        {
            if (colorDistance(seed, input.at<Vec3b>(x + 1, y - 1), colorThreshold, whiteThreshold))
            {
                //cout << "reached Upper right pixel" << endl;
                reach[x + 1][y - 1] = true;
                s = intToString(x + 1, y - 1);
                queue.push_back(s);
                //updateMean(seed, input.at<Vec3b>(x + 1, y - 1), count);
                modifyPixel(filled, x + 1, y - 1, colorflag);
                ++count;
            }
            else
                modifyPixel(edgeMap, x + 1, y - 1, colorflag);
        }

        //Bottom Left Pixel
        if (x - 1 >= 0 && y + 1 < input.cols && (!reach[x - 1][y + 1]))
        {
            if (colorDistance(seed, input.at<Vec3b>(x - 1, y + 1), colorThreshold, whiteThreshold))
            {
                //cout << "reached Bottom left pixel" << endl;			
                reach[x - 1][y + 1] = true;
                s = intToString(x - 1, y + 1);
                queue.push_back(s);
                //updateMean(seed, input.at<Vec3b>(x - 1, y + 1), count);
                modifyPixel(filled, x - 1, y + 1, colorflag);
                ++count;
            }
            else
                modifyPixel(edgeMap, x - 1, y + 1, colorflag);
        }

        //Upper left Pixel
        if (x - 1 >= 0 && y - 1 >= 0 && (!reach[x - 1][y - 1]))
        {
            if (colorDistance(seed, input.at<Vec3b>(x - 1, y - 1), colorThreshold, whiteThreshold))
            {
                //cout << "reached Upper left pixel" << endl;
                reach[x - 1][y - 1] = true;
                s = intToString(x - 1, y - 1);
                queue.push_back(s);
                //updateMean(seed, input.at<Vec3b>(x - 1, y - 1), count);
                modifyPixel(filled, x - 1, y - 1, colorflag);
                ++count;
            }
            else
                modifyPixel(edgeMap, x - 1, y - 1, colorflag);
        }
    }
}

/*
        Funtion to set both the thresholds	
 */
void grow::setThresholds(int colorThreshold, int whiteThreshold)
{
    this->colorThreshold = colorThreshold;
    this->whiteThreshold = whiteThreshold;
}
/*
        Destructor
//*/
//grow::~grow()
//{
//
//}