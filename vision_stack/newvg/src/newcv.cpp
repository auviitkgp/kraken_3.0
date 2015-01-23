#include <newcv/newcv.h>
#include <blob/blob.h>
#include <blob/BlobContour.h>
#include <blob/BlobProperties.h>
#include <blob/BlobResult.h>
#include <blob/ComponentLabeling.h>
#include <blob/BlobLibraryConfiguration.h>
#include <blob/BlobOperators.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <ros/ros.h>



using namespace std;
using namespace cv;
using namespace ros;


int main( int argc, char** argv )
{
   Scalar _low(10,0,0) , _high(100,255,255);
   Mat _img,_canny, _hsv,_gray,_dilateImg,_nlimg,_thresh,_median;
   double _epsilon=1;
   vector<Vec4i>  _lines;
   vector<Point> _corners;
   double _threshold1, _threshold2;
   vector<vector<Point> > _contours;
   CBlobResult _blobs1,_blobs2;
   CBlob * _currentBlob1,*_currentBlob2;
   CBlob cb;
   Point _centerImg,_error, _centerLine;
   double _errorAngle;

   int _xmin, _ymin, _xmax, _ymax, x1,y1,x2,y2;
//   IplImage Iipl;
   Mat _kernel = getStructuringElement(MORPH_RECT,Size(3,3),Point(0,0));
   VideoCapture _cap("/home/manvi/ros_ws/newcv/bin/forward-wire-15_58_35.avi");

   CBlobGetMean cbgm1,cbgm2;

   if(!_cap.isOpened())
      return 0;

   while(1){
       _cap >> _img;
       cvtColor(_img,_gray,CV_BGR2GRAY);
       cvtColor(_img,_hsv,CV_BGR2HSV_FULL);
//       imshow("hsv",_hsv);
       inRange(_hsv,_low,_high,_thresh);
       medianBlur(_thresh,_median,3);
//       imshow("_median", _median);

//       fastNlMeansDenoising(_median,_nlimg);
       erode(_median,_dilateImg,_kernel);
       dilate(_dilateImg,_dilateImg,_kernel);
       Canny(_dilateImg,_canny,20,200,3);


//       goodFeaturesToTrack(_dilateImg,corners,100,0.01,20);
//       cout <<"total number of corners " << corners.size() << endl;

//       for(int i=0; i<corners.size() ; i++){
//           circle(_dilateImg,corners[i],3,Scalar(255,0,0));
//       }


//       imshow("_dilateImg",_dilateImg);

       findContours(_canny,_contours,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
       cout << "_contours size= " << _contours.size() << endl;
       drawContours(_canny,_contours,-1,Scalar(255,0,0));
       imshow("median",_median);

//       for(int i=0; i<contours.size();i++){
//           drawContours(median,contours,i,Scalar(0,0,255));
//       }

//       imshow("_canny",_canny);
//       imshow("org",median);
//       imshow("dilate",_dilateImg);

       IplImage fimage = _median;
       _blobs1 = CBlobResult(&fimage,NULL,0);
       _blobs2 = CBlobResult(&fimage,NULL,0);
       _blobs1.Filter(_blobs1, B_EXCLUDE, CBlobGetArea(), B_GREATER, 100);
       _blobs2.Filter(_blobs2,B_INCLUDE,CBlobGetArea(), B_LESS, 20);
       cout << "blob1 size = " << _blobs1.GetNumBlobs() << endl;

       cout << "blob2 = " << _blobs2.GetNumBlobs() << endl;
//       //Box2D e=CBlob.GetEllipse();
//       //CBlobGetArea.GetNom();

////       vector<Rect> r(_blobs1.GetNumBlobs());

       for(int i=0; i< _blobs1.GetNumBlobs();i++){

               _currentBlob1 = _blobs1.GetBlob(i);
               _currentBlob1->FillBlob(&fimage,Scalar(0));
           }

           for (int i = 0; i < _blobs2.GetNumBlobs(); i++)
           {
               _currentBlob2=_blobs2.GetBlob(i);
               // Making the clutters black
               //currentBlob->GetBoundingBox();
               _currentBlob2->FillBlob(&fimage,Scalar(255));
          }


//////       //blobs3=CBlobResult(&fimage,NULL,0);
       Mat fmat(&fimage);
//       imshow("org",_img);
//       imshow("fmat",fmat);
//       imshow("fimg",fmat);
       Canny(fmat,_canny,20,200);
//       imshow("canny",_canny);
//        HoughLinesP()
       HoughLinesP(fmat,_lines,1,CV_PI/180,100,30,20);

       if(_lines.size() == 0){
//           cout << "no hough lines...";
           continue;
       }
//       if(lines.size() < 4){
//           cout << "hough lines not detected..! " << endl;
//       }
//       else if(lines.size() == 4){
//           cout << "4 lines detected.!" << endl;
//       }
//       else
//           cout << "more than 4 hough lines detected..! " << endl;


       _xmin = _lines[0][0];
       _xmax = _lines[0][0];
       _ymin = _lines[0][0];
       _ymax = _lines[0][0];

       for(int i=0; i<_lines.size(); i++){
           line(fmat,Point(_lines[i][0],_lines[i][1]),Point(_lines[i][2],_lines[i][3]),Scalar(0,0,255),1);
           if(_lines[i][0] < _lines[i][2]){
               if(_xmin > _lines[i][0]){
                   _xmin = _lines[i][0];
                   y1 = _lines[i][1];
               }
               if(_xmax < _lines[i][2]){
                   _xmax = _lines[i][2];
                   y2 = _lines[i][3];
               }
           }else {
               if(_xmin > _lines[i][2]){
                   _xmin = _lines[i][2];
                   y1 = _lines[i][3];
               }
               if(_xmax < _lines[i][0]){
                   _xmax = _lines[i][0];
                   y2 = _lines[i][1];
               }
           }
           if(_lines[i][1] < _lines[i][3]){
               if(_ymin > _lines[i][1]){
                   _ymin = _lines[i][1];
                   x1 = _lines[i][0];
               }
               if(_ymax < _lines[i][3]){
                   _ymax = _lines[i][3];
                   x2 = _lines[i][2];
               }
           }else{
               if(_ymin > _lines[i][3]){
                   _ymin = _lines[i][3];
                   x1 = _lines[i][2];
               }
               if(_ymax < _lines[i][1]){
                   _ymax = _lines[i][1];
                   x2 = _lines[i][0];
               }
           }
       }


       line(fmat,Point(_xmin,y1),Point(x1,_ymin),Scalar(255,0,0));
//       imshow("fimg",fmat);
       vector<RotatedRect> _rect;
       _centerImg.x = fmat.rows/2;
       _centerImg.y = fmat.cols/2;
       _centerLine.x = (_xmin+x1)/2;
       _centerLine.y = (y1+_ymin)/2;
       cout << "centerline point = " << _centerLine.x << " " << _centerLine.y << endl;

       _error.x = _centerImg.x - _centerLine.x;
       _error.y = _centerImg.y - _centerLine.y;
       _errorAngle = atan2((_ymin-y1),(x2-_xmin))*180/CV_PI;

       cout << "error = " << _error.x << " " << _error.y << endl;

       if(_error.x > 10)
       {
           cout << "move to left" << endl;
       }else if(_error.x < -10)
           cout << "move to right..!" << endl;
       if(_error.y > 10)
            cout << "go upwards.!" << endl;
       else if(_error.y < -10)
           cout << "go downwards.!" << endl;
       if(_errorAngle > 55 || _errorAngle < 35)
           cout << "rotate the gate..!" << endl;

       imshow("fmat", fmat);
       if(waitKey(33)==27)
           break;

    }
                                            // Wait for a keystroke in the window
   return 0;
}
