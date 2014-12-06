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
   Scalar low(10,0,0) , high(100,255,255);
   Mat img,canny, hsv,gray,dilate_img,nlimg,thresh,median;
   double epsilon=1;
   vector<Vec4i>  lines;
   vector<Point> corners;
   double threshold1, threshold2;
   vector<vector<Point> > contours;
   CBlobResult blobs1,blobs2;
   CBlob * currentBlob1,*currentBlob2;
   CBlob cb;
   Point _centerImg,error, _centerLine;
   double _errorAngle;

   int xmin, ymin, xmax, ymax, x1,y1,x2,y2;
//   IplImage Iipl;
   Mat kernel = getStructuringElement(MORPH_RECT,Size(3,3),Point(0,0));
   VideoCapture cap("/home/manvi/ros_ws/newcv/bin/forward-wire-15_58_35.avi");

   CBlobGetMean cbgm1,cbgm2;

   if(!cap.isOpened())
      return 0;

   while(1){
       cap >> img;
       cvtColor(img,gray,CV_BGR2GRAY);
       cvtColor(img,hsv,CV_BGR2HSV_FULL);
//       imshow("hsv",hsv);
       inRange(hsv,low,high,thresh);
       medianBlur(thresh,median,3);
//       imshow("median", median);

//       fastNlMeansDenoising(median,nlimg);
       erode(median,dilate_img,kernel);
       dilate(dilate_img,dilate_img,kernel);
       Canny(dilate_img,canny,20,200,3);


//       goodFeaturesToTrack(dilate_img,corners,100,0.01,20);
//       cout <<"total number of corners " << corners.size() << endl;

//       for(int i=0; i<corners.size() ; i++){
//           circle(dilate_img,corners[i],3,Scalar(255,0,0));
//       }


//       imshow("dilate_img",dilate_img);

       findContours(canny,contours,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
       cout << "contours size= " << contours.size() << endl;
       drawContours(canny,contours,-1,Scalar(255,0,0));
       imshow("median",median);

//       for(int i=0; i<contours.size();i++){
//           drawContours(median,contours,i,Scalar(0,0,255));
//       }

//       imshow("canny",canny);
//       imshow("org",median);
//       imshow("dilate",dilate_img);

       IplImage fimage = median;
       blobs1 = CBlobResult(&fimage,NULL,0);
       blobs2 = CBlobResult(&fimage,NULL,0);
       blobs1.Filter(blobs1, B_EXCLUDE, CBlobGetArea(), B_GREATER, 100);
       blobs2.Filter(blobs2,B_INCLUDE,CBlobGetArea(), B_LESS, 20);
       cout << "blob1 size = " << blobs1.GetNumBlobs() << endl;

       cout << "blob2 = " << blobs2.GetNumBlobs() << endl;
//       //Box2D e=CBlob.GetEllipse();
//       //CBlobGetArea.GetNom();

////       vector<Rect> r(blobs1.GetNumBlobs());

       for(int i=0; i< blobs1.GetNumBlobs();i++){

               currentBlob1 = blobs1.GetBlob(i);
               currentBlob1->FillBlob(&fimage,Scalar(0));
           }

           for (int i = 0; i < blobs2.GetNumBlobs(); i++)
           {
               currentBlob2=blobs2.GetBlob(i);
               // Making the clutters black
               //currentBlob->GetBoundingBox();
               currentBlob2->FillBlob(&fimage,Scalar(255));
          }


//////       //blobs3=CBlobResult(&fimage,NULL,0);
       Mat fmat(&fimage);
//       imshow("org",img);
//       imshow("fmat",fmat);
//       imshow("fimg",fmat);
       Canny(fmat,canny,20,200);
//       imshow("canny",canny);
//        HoughLinesP()
       HoughLinesP(fmat,lines,1,CV_PI/180,100,30,20);

       if(lines.size() == 0){
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


       xmin = lines[0][0];
       xmax = lines[0][0];
       ymin = lines[0][0];
       ymax = lines[0][0];

       for(int i=0; i<lines.size(); i++){
           line(fmat,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]),Scalar(0,0,255),1);
           if(lines[i][0] < lines[i][2]){
               if(xmin > lines[i][0]){
                   xmin = lines[i][0];
                   y1 = lines[i][1];
               }
               if(xmax < lines[i][2]){
                   xmax = lines[i][2];
                   y2 = lines[i][3];
               }
           }else {
               if(xmin > lines[i][2]){
                   xmin = lines[i][2];
                   y1 = lines[i][3];
               }
               if(xmax < lines[i][0]){
                   xmax = lines[i][0];
                   y2 = lines[i][1];
               }
           }
           if(lines[i][1] < lines[i][3]){
               if(ymin > lines[i][1]){
                   ymin = lines[i][1];
                   x1 = lines[i][0];
               }
               if(ymax < lines[i][3]){
                   ymax = lines[i][3];
                   x2 = lines[i][2];
               }
           }else{
               if(ymin > lines[i][3]){
                   ymin = lines[i][3];
                   x1 = lines[i][2];
               }
               if(ymax < lines[i][1]){
                   ymax = lines[i][1];
                   x2 = lines[i][0];
               }
           }
       }


       line(fmat,Point(xmin,y1),Point(x1,ymin),Scalar(255,0,0));
//       imshow("fimg",fmat);
       vector<RotatedRect> _rect;
       _centerImg.x = fmat.rows/2;
       _centerImg.y = fmat.cols/2;
       _centerLine.x = (xmin+x1)/2;
       _centerLine.y = (y1+ymin)/2;
       cout << "centerline point = " << _centerLine.x << " " << _centerLine.y << endl;

       error.x = _centerImg.x - _centerLine.x;
       error.y = _centerImg.y - _centerLine.y;
       _errorAngle = atan2((ymin-y1),(x2-xmin))*180/CV_PI;

       cout << "error = " << error.x << " " << error.y << endl;

       if(error.x > 10)
       {
           cout << "move to left" << endl;
       }else if(error.x < -10)
           cout << "move to right..!" << endl;
       if(error.y > 10)
            cout << "go upwards.!" << endl;
       else if(error.y < -10)
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
