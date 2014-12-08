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

const int _THRES_LINE_GAP=15;
Mat _img;
struct bunch{
    Point p;
    vector<cv::Vec4i> v;
};
pair<bunch,pair<bunch,bunch> > getLargestBunch(const vector<Vec4i> &lines);

void printbunch( vector<bunch> &b, string name)
{
    Mat img(500, 500, CV_8UC3, Scalar(0,0,255));;
    for(int j=0; j<b.size();j++){
        vector<Vec4i> &v = b[j].v;


        for(int i=0; i<v.size(); i++ ){
            line(img,Point(v[i][0],v[i][1]),Point(v[i][2],v[i][3]),Scalar(255,0,0));
        }
    }
    imshow(name,img);
    if(waitKey(33) == 27)
        exit(0);
}

int main( int argc, char** argv )
{
    if(argc < 2){
        cout << "give video path";
        return 0;
    }
//   Scalar _low(10,0,0) , _high(100,255,255);
   Mat _canny, _hsv,_gray,_dilateImg,_thresh,_median;
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

   Mat _kernel = getStructuringElement(MORPH_RECT,Size(3,3),Point(0,0));
   VideoCapture _cap(argv[1]);

   CBlobGetMean cbgm1,cbgm2;

   if(!_cap.isOpened())
      return 0;

   while(1){
       _cap >> _img;
       cvtColor(_img,_hsv,CV_BGR2GRAY);
//       inRange(_hsv,_low,_high,_thresh);
//       medianBlur(_thresh,_median,3);
       adaptiveThreshold(_hsv,_median,200,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,13,0);
       imshow("adaptive", _median);
       erode(_median,_dilateImg,_kernel);
       dilate(_dilateImg,_dilateImg,_kernel);
       imshow("erosion",_dilateImg);
//       Canny(_dilateImg,_canny,20,200,3);

       IplImage fimage = _dilateImg;
       _blobs1 = CBlobResult(&fimage,NULL,0);
       _blobs2 = CBlobResult(&fimage,NULL,0);
       _blobs1.Filter(_blobs1, B_EXCLUDE, CBlobGetArea(), B_GREATER, 100);
       _blobs2.Filter(_blobs2,B_INCLUDE,CBlobGetArea(), B_LESS, 40);


       for(int i=0; i< _blobs1.GetNumBlobs();i++){
               _currentBlob1 = _blobs1.GetBlob(i);
               _currentBlob1->FillBlob(&fimage,Scalar(255));
           }

           for (int i = 0; i < _blobs2.GetNumBlobs(); i++)
           {
               _currentBlob2=_blobs2.GetBlob(i);
               _currentBlob2->FillBlob(&fimage,Scalar(0));
          }

       Mat fmat(&fimage);

       imshow("after blob detection",fmat);

       HoughLinesP(fmat,_lines,1,CV_PI/180,60,30,10);          // check line length, threshold and max gap between points

       pair<bunch,pair<bunch,bunch> > retpair = getLargestBunch(_lines);
       bunch b=retpair.first;

       vector<Vec4i> &v = b.v;



        for(int i=0; i<v.size(); i++){
            line(_img,Point(v[i][0],v[i][1]),Point(v[i][2],v[i][3]),Scalar(0,255,0));
        }



        bunch &hor=retpair.second.first,&ver=retpair.second.second;

        int xmin, xmax, ymin, ymax,x1,y1,x2,y2;
        xmin = 700;
        xmax = 0;
        ymin = 700;
        ymax = 0;

        for(int i=0; i<ver.v.size(); i++){
            int c,d;
               if(ver.v[i][1] != ver.p.y){

                       d = ver.v[i][1];
                       c = ver.v[i][0];

               }else{

                       d = ver.v[i][3];
                       c = ver.v[i][2];

               }
               if(ymin > d) {
                   ymin = d;
                   x1 =c;
               }

        }

        for(int i=0; i<hor.v.size(); i++){
            int a,b;
            if(hor.v[i][0] != hor.p.x){
                a = hor.v[i][0];
                b = hor.v[i][1];
            }
            else{
                a = hor.v[i][2];
                b = hor.v[i][3];
            }
            if(xmin > a){
                xmin = a;
                y1 = b;
            }

        }


        line(_img,Point(xmin,y1),Point(x1,ymin),Scalar(255,0,0));
        imshow("original", _img);


//            line(fmat,Point(_xmin,y1),Point(x1,_ymin),Scalar(255,0,0));
//            line(_img,Point(_xmin,y1),Point(x1,_ymin),Scalar(255,0,0));
//            _centerImg.x = fmat.rows/2;
//            _centerImg.y = fmat.cols/2;
//            _centerLine.x = (_xmin+x1)/2;
//            _centerLine.y = (y1+_ymin)/2;
//            cout << "centerline point = " << _centerLine.x << " " << _centerLine.y << endl;

//            _error.x = _centerImg.x - _centerLine.x;
//            _error.y = _centerImg.y - _centerLine.y;
//            _errorAngle = atan2((_ymin-y1),(x2-_xmin))*180/CV_PI;

//            cout << "error = " << _error.x << " " << _error.y << endl;

//            if(_error.x > 10)
//            {
//                cout << "move to left" << endl;
//            }else if(_error.x < -10)
//                cout << "move to right..!" << endl;
//            if(_error.y > 10)
//                 cout << "go upwards.!" << endl;
//            else if(_error.y < -10)
//                cout << "go downwards.!" << endl;
//            if(_errorAngle > 55 || _errorAngle < 35)
//                cout << "rotate the gate..!" << endl;
//            imshow("img", _img);
//            imshow("fmat", fmat);
//            imshow("hough lines",_img);

       if(waitKey(33)==27)
           break;

    }

   return 0;
}



inline bool withinThres(int th,int a ,int b)
{
    if(fabs(b-a)<th)
        return true;
    else
        return false;
}


pair<bunch,pair<bunch,bunch> > getLargestBunch(const vector<Vec4i> &lines)
{
    vector<bunch> bunches_x,bunches_y;
    for (int i = 0; i < lines.size(); ++i) {
        double theta;
        if(lines[i][2]-lines[i][0]!=0)
            theta = atan((lines[i][3]-lines[i][1])/(lines[i][2]-lines[i][0]));
        else continue;

        if(theta>CV_PI/180*-30&&theta<30*CV_PI/180)
        {

            Point right;
            if(lines[i][0]<lines[i][2])
                right.x=lines[i][2],right.y=lines[i][3];
            else
                right.x=lines[i][0],right.y=lines[i][1];
            bool added=false;
            for (int j = 0; j <bunches_x.size() ; ++j) {
                   Point bunchpt=bunches_x[j].p;

                   if(withinThres(_THRES_LINE_GAP,bunchpt.x,right.x)&&withinThres(_THRES_LINE_GAP,bunchpt.y,right.y))
                   {
                       bunches_x[j].v.push_back(lines[i]);
                       added=true;
                   }
            }
            if(!added){
                bunch b;
                b.p=right;
                b.v.push_back(lines[i]);
                bunches_x.push_back(b);
            }

}
        if(theta >CV_PI/180*60 && theta < CV_PI/180*120){
            Point low;

            if(lines[i][1]<lines[i][3])
                low.x=lines[i][2],low.y=lines[i][3];
            else
                low.x=lines[i][0],low.y=lines[i][1];
            bool added=false;
            for (int j = 0; j <bunches_y.size() ; ++j) {
                   Point bunchpt=bunches_y[j].p;
                   int thre=15;
                   if(withinThres(thre,bunchpt.x,low.x)&&withinThres(thre,bunchpt.y,low.y))
                   {
                       bunches_y[j].v.push_back(lines[i]);
                       added=true;
                   }
            }
            if(!added){
                bunch b;
                b.p=low;
                b.v.push_back(lines[i]);
                bunches_y.push_back(b);
            }
        }
    }
    printbunch(bunches_x,"bunch X");
    printbunch(bunches_y,"bunches Y");

    bunch bmax;

    int sum=0;
    pair<bunch,bunch> sendpair;
    for(int i=0;i<bunches_x.size();i++)
    {

        for (int j = 0; j < bunches_y.size(); ++j) {
            if(withinThres(20,bunches_x[i].p.x,bunches_y[j].p.x)&&withinThres(20,bunches_x[i].p.y,bunches_y[j].p.y))
              {

                int pre=sum;
                sum=std::max(sum,(int)(bunches_x[i].v.size()+bunches_y[j].v.size()));
                bmax.p=bunches_x[i].p;
                bmax.v=bunches_x[i].v;
                bmax.v.insert(bmax.v.end(),bunches_y[j].v.begin(),bunches_y[j].v.end());
                if(pre!=sum)
                {
                    sendpair.first=bunches_x[i];sendpair.second=bunches_y[j];
                }
            }
        }
    }
    return make_pair(bmax,sendpair);
}
