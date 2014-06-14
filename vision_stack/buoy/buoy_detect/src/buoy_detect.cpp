#include <ros/ros.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <iostream>
#include <fstream>
#include <blob/blob.h>
#include <blob/BlobContour.h>
#include <blob/BlobLibraryConfiguration.h>
#include <blob/BlobOperators.h>
#include <blob/BlobProperties.h>
#include <blob/BlobResult.h>
#include <blob/ComponentLabeling.h>

using namespace std;
using namespace cv;

int main(int argc, char ** argv)
{
	ros::init(argc, argv, "buoydetect");

    VideoCapture _camera("/home/madhukar/videolog/forward-buoys-15_56_53.avi");
    Mat _image, _imageHSV, _imageBW;
    vector<vector<Point> > _contours;
    Mat _kernelDilateErode = getStructuringElement(MORPH_RECT, Size(3,3));
    while(ros::ok())
    {
        _camera >> _image;
        cvtColor(_image, _imageHSV, CV_BGR2HSV_FULL);
        inRange(_imageHSV, Scalar(0,0,0),Scalar(20,255,255), _imageBW);
        medianBlur(_imageBW, _imageBW, 3);
        erode(_imageBW, _imageBW, _kernelDilateErode);
        CBlobResult _blobs,_blobsClutter;
        CBlob * _currentBlob;
        IplImage _imageBWipl = _imageBW;
        _blobs = CBlobResult(&_imageBWipl, NULL, 0);
        _blobs.Filter(_blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 2000);
        for(int i = 0; i < _blobs.GetNumBlobs(); i++)
        {
            _currentBlob = _blobs.GetBlob(i);
            _currentBlob->FillBlob(&_imageBWipl, Scalar(0));
        }
        _blobsClutter = CBlobResult(&_imageBWipl, NULL, 0);
        _blobsClutter.Filter(_blobsClutter, B_EXCLUDE, CBlobGetArea(), B_GREATER, 50);
        for(int i = 0; i < _blobsClutter.GetNumBlobs(); i++)
        {
            _currentBlob = _blobsClutter.GetBlob(i);
            _currentBlob->FillBlob(&_imageBWipl, Scalar(0));
        }

        medianBlur(_imageBW, _imageBW, 5);
        imshow("Buoy", _imageBW);
        findContours(_imageBW, _contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

        vector<vector<Point> > _contoursPoly(_contours.size());
        vector<Point2f> _center(_contours.size());
        vector<float> _radius(_contours.size());
        for(int i=0; i < _contours.size(); i++)
        {
            if(contourArea(_contours[i])>100)
            {
               approxPolyDP(_contours[i],_contoursPoly[i],3,true);
               minEnclosingCircle((Mat)_contoursPoly[i],_center[i],_radius[i]);
               circle(_image,_center[i],_radius[i],Scalar(255,0,0));
            }
        }

        imshow("final image", _image);
        if(waitKey(33) == 27)
            break;
    }

	return 0;
}
