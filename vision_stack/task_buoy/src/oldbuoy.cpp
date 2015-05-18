#include <ros/ros.h>
#include <task_buoy/buoy_server.h>
#define PI 3.1414

#include <resources/topicHeader.h>


class maxRGB
{



public:

    /** main function to call for maxRGB color correction **/
    Mat  run(Mat,int p,int m);


    /** function coputes the illumination estimate max RGB algorithm.The function takes input image,minkowski norm and normalization method .
    *   the output are ml,,ma,mb for each of the color channels
    **/

    void  process(Mat src1,float *ml,float *ma,float *mb,int p,int m);


};

class max_edge
{
public:
    /** main function to call to perform max-edge color correction */
    Mat  run(Mat,int p,int m);

    /** defines convolution type **/
    enum ConvolutionType {
    /* Return the full convolution, including border */
      CONVOLUTION_FULL,

    /* Return only the part that corresponds to the original image */
      CONVOLUTION_SAME,

    /* Return only the submatrix containing elements that were not influenced by the border */
      CONVOLUTION_VALID
    };

    /** method to perform 2D convolution */
    void conv2(const Mat &img, const Mat& kernel, ConvolutionType type, Mat& dest);

    /** function that computes illumination vector for max_edge algorithm**/
    void  process(Mat src1,float *ml,float *ma,float *mb,int p,int m);
};









Buoy::Buoy(std::string name) : _it(_n), _s(_n, name, boost::bind(&Buoy::executeCB, this, _1), false), _actionName(name)
{
    _sub = _it.subscribe(topics::CAMERA_FRONT_RAW_IMAGE, 1, &Buoy::imageCallBack, this);
    _pub = _it.advertise(topics::CAMERA_FRONT_BUOY_IMAGE, 1);

    ifstream _thresholdVal("threshold.th");
    if(_thresholdVal.is_open())
    {
        for(int i = 0; i < 3; i++)
        {
            _thresholdVal >> _lowerThreshRed1[i];
        }
        for(int i = 0; i < 3; i++)
        {
            _thresholdVal >> _upperThreshRed1[i];
        }
        for(int i = 0; i < 3; i++)
        {
            _thresholdVal >> _lowerThreshRed2[i];
        }
        for(int i = 0; i < 3; i++)
        {
            _thresholdVal >> _upperThreshRed2[i];
        }
        for(int i = 0; i < 3; i++)
        {
            _thresholdVal >> _lowerThreshGreen[i];
        }
        for(int i = 0; i < 3; i++)
        {
            _thresholdVal >> _upperThreshGreen[i];
        }
    }
    else
    {
        ROS_ERROR("Unable to open threshold file.");
        ros::shutdown();
    }
    _kernelDilateErode = getStructuringElement(MORPH_RECT, Size(3,3));
    _finalImage.encoding = "mono8";
    _s.start();
}

void Buoy::imageCallBack(const sensor_msgs::ImageConstPtr &_msg)
{
    cv_bridge::CvImagePtr _imagePtr;
    try
    {
        _imagePtr = cv_bridge::toCvCopy(_msg, "bgr8");
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", _msg->encoding.c_str());
    }
    _image = _imagePtr->image;
}

void Buoy::executeCB(const actionmsg::buoyGoalConstPtr &_goal)
{
    ros::Rate looprate(10);
    bool success = true;

    cout <<"<<<<<<   _goal->order =   >>>>> " << _goal-> order;

    switch(_goal->order)
    {
        case DETECT_BUOY:
        {
            bool _detected = false;
            while(ros::ok())
            {
                if (_s.isPreemptRequested() || !ros::ok())
                {
                    ROS_INFO("%s: Preempted", _actionName.c_str());
                    _s.setPreempted();
                    success = false;
                    break;
                }
                _detected = detectBuoy();
                _finalImage.image = _imageBW;
                _finalImagemsg = _finalImage.toImageMsg();
                _pub.publish(_finalImagemsg);

                if(_detected)
                {
                    _result.sequence.push_back(BUOY_DETECTED);
                    cout << "<<<<<<<<<< buoy has been detected..  >>>>>>>>>>>>>"; 
                    break;
                }
                looprate.sleep();
            }
            break;
        }

        case ALIGN_BUOY:
        {
            while(ros::ok())
            {
                if (_s.isPreemptRequested() || !ros::ok())
                {
                    ROS_INFO("%s: Preempted", _actionName.c_str());
                    _s.setPreempted();
                    success = false;
                    break;
                }

                detectBuoy();
                getAllignment();
                _finalImage.image = _imageBW;
                _finalImagemsg = _finalImage.toImageMsg();
                _pub.publish(_finalImagemsg);
                if((_feedback.errorx < 10 && _feedback.errorx > -10) && (_feedback.errory > -10 && _feedback.errory < 10))
                {
                    _result.sequence.push_back(BUOY_ALIGNED);
                    cout << "<<<<<<<<<<<<  BUOY_ALIGNED   >>>>>>>>>>>>>>" << endl;
                    break;
                }
                _s.publishFeedback(_feedback);
                looprate.sleep();
            }
            break;
        }
    }

    if(success)
    {
        _result.sequence.push_back(_goal->order);
        _s.setSucceeded(_result);
    }
}



//white balance

Mat  color_correction::maxRGB::run(Mat src1,int p,int m)
{

    Mat dst;

   src1.copyTo(dst);

    cv::Mat_<cv::Vec3b>::const_iterator it= src1.begin<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::const_iterator itend= src1.end<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itout= dst.begin<cv::Vec3b>();

    float ma=0,mb=0,ml=0;
    process(src1,&ml,&ma,&mb,p,m);
    for ( ; it!= itend; ++it, ++itout)
    {


        cv::Vec3b v1=*it;

        float l=v1.val[0];
        float a=v1.val[1];
        float b=v1.val[2];

        if(m==0)
        {
        a=a*(ma);
        b= b*(mb);
        l= l*(ml);
        }
        else
        {
        //if(a<(float)95*255/100)
        a=a*(ma);
        //if(b<(float)95*255/100)
        b= b*(mb);
       // if(l<(float)95*255/100)
        l= l*(ml);
        }

        if(a>255)
            a=255;
        if(b>255)
            b=255;
        if(l>255)
            l=255;


        v1.val[0]=l;

        v1.val[1]=a;

        v1.val[2]=b;

        *itout=v1;



    }

    return dst;
}



void  color_correction::maxRGB::process(Mat src1,float *ml,float *ma,float *mb,int p,int m)
{

    Mat src;

    src1.convertTo(src,CV_32FC3,1.0,0);

    *ma=0;
    *mb=0;
    *ml=0;

      for(int i=0;i<src.rows;i++)
      {

          for(int j=0;j<src.cols;j++)
          {
              Vec3f v1=src.at<cv::Vec3f>(i,j);
              double lc=v1.val[0];
              double ac=v1.val[1];
              double bc=v1.val[2];
              *ml=max((double)*ml,(double)lc);
              *ma=max((double)*ma,(double)ac);
              *mb=max((double)*mb,bc);

          }
      }





      if(m==1)
      {
              double r=(*ma+*mb+*ml)/3;
            r=(double)sqrt((double)(*ma)*(*ma)+(double)(*mb)*(*mb)+(double)(*ml)*(*ml));
           *ma=(double)(*ma)/(double)r;
            *mb=(double)(*mb)/(double)r;
            *ml=(double)(*ml)/(double)r;


            r=max((double)*ma,(double)*mb);
            r=max((double)r,(double)*ml);

            *ma=(double)(r)/(double)*ma;
            *mb=(double)(r)/(double)*mb;
            *ml=(double)(r)/(double)*ml;



      }
    else
      {


          cerr <<  *ml << endl;
          cerr <<  *ma << endl;
          cerr <<  *mb << endl;

          double r=max((double)*ml,(double)*ma);
          r=max((double)r,(double)*mb);
          *ma=(double)(r)/(double)*ma;
          *mb=(double)(r)/(double)*mb;
          *ml=(double)(r)/(double)*ml;


      }


}



Mat  color_correction::max_edge::run( Mat src1,int p,int m)
{

    Mat dst;

   src1.copyTo(dst);

    cv::Mat_<cv::Vec3b>::const_iterator it= src1.begin<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::const_iterator itend= src1.end<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itout= dst.begin<cv::Vec3b>();

    float ma=0,mb=0,ml=0;
    process(src1,&ml,&ma,&mb,p,m);
    for ( ; it!= itend; ++it, ++itout)
    {


        cv::Vec3b v1=*it;

        float l=v1.val[0];
        float a=v1.val[1];
        float b=v1.val[2];

        if(m==0)
        {
        a=a*(ma);
        b= b*(mb);
        l= l*(ml);
        }
        else
        {
        //if(a<(float)95*255/100)
        a=a*(ma);
        //if(b<(float)95*255/100)
        b= b*(mb);
       // if(l<(float)95*255/100)
        l= l*(ml);
        }

        if(a>255)
            a=255;
        if(b>255)
            b=255;
        if(l>255)
            l=255;


        v1.val[0]=l;

        v1.val[1]=a;

        v1.val[2]=b;

        *itout=v1;



    }

   return dst;
}



void color_correction::max_edge::conv2(const Mat &img, const Mat& kernel, ConvolutionType type, Mat& dest) {
  Mat source = img;
  if(CONVOLUTION_FULL == type) {
    source = Mat();
    const int additionalRows = kernel.rows-1, additionalCols = kernel.cols-1;
    copyMakeBorder(img, source, (additionalRows+1)/2, additionalRows/2, (additionalCols+1)/2, additionalCols/2, BORDER_CONSTANT, Scalar(0));
  }

  Point anchor(kernel.cols - kernel.cols/2 - 1, kernel.rows - kernel.rows/2 - 1);
  int borderMode = BORDER_CONSTANT;
  flip(kernel,kernel,-1);
  filter2D(source, dest, img.depth(),kernel, anchor, 0, borderMode);

  if(CONVOLUTION_VALID == type) {
    dest = dest.colRange((kernel.cols-1)/2, dest.cols - kernel.cols/2)
               .rowRange((kernel.rows-1)/2, dest.rows - kernel.rows/2);
  }
}


void  color_correction::max_edge::process(Mat src1,float *ml,float *ma,float *mb,int p,int m)
{

    *ma=0;
    *mb=0;
    *ml=0;


      Mat src;
      vector<Mat> bgr_planes;
      Mat src2;
        //src1.convertTo(src2,CV_32FC3,1.0/255.0,0);
      src1.convertTo(src,CV_32FC3,1.0,0);
      split( src, bgr_planes );

      Mat kernel=getGaussianKernel(100,6,CV_32F);


      Mat grad_x, grad_y,abs_angle_x;
      Mat grad1,grad2,grad3;
      Mat r1,g,b;

      cerr << bgr_planes[0].depth() << ":" << CV_32FC1<<  "dddddd" << endl;

      conv2( bgr_planes[0], kernel, CONVOLUTION_FULL,r1);
      conv2( bgr_planes[1], kernel, CONVOLUTION_FULL,g);
      conv2( bgr_planes[2], kernel, CONVOLUTION_FULL,b);

      //cv::imshow("dddd",b);
      // 1 2 1      1 0 -1
      // 0 0 0      2 0 -2
      // -1 -2 -1   1 0 -1
      double dx[]={1,2,1,0,0,0,-1,-2,-1};
      double dy[]={1,0,-1,2,0,-2,1,0,-1};

      Mat kx=Mat(3,3,CV_32FC1,dx);
      Mat ky=Mat(3,3,CV_32FC1,dy);


      grad_x.setTo(cv::Scalar::all(0));
      grad_y.setTo(cv::Scalar::all(0));

      conv2(r1, kx, CONVOLUTION_FULL,grad_x);
      conv2(r1, ky, CONVOLUTION_FULL,grad_y);


     cartToPolar(grad_x,grad_y,bgr_planes[0],abs_angle_x,false);
     grad_x.setTo(cv::Scalar::all(0));
     grad_y.setTo(cv::Scalar::all(0));

      conv2(g, kx, CONVOLUTION_FULL,grad_x);
      conv2(g, ky, CONVOLUTION_FULL,grad_y);

      cartToPolar(grad_x,grad_y,bgr_planes[1],abs_angle_x,false);
      grad_x.setTo(cv::Scalar::all(0));
      grad_y.setTo(cv::Scalar::all(0));

      conv2(b, kx, CONVOLUTION_FULL,grad_x);
      conv2(b, ky, CONVOLUTION_FULL,grad_y);

      cartToPolar(grad_x,grad_y,bgr_planes[2],abs_angle_x,false);







      cv::merge(bgr_planes,src);

      for(int i=0;i<src.rows;i++)
      {

          for(int j=0;j<src.cols;j++)
          {
              Vec3f v1=src.at<cv::Vec3f>(i,j);
              double lc=v1.val[0];
              double ac=v1.val[1];
              double bc=v1.val[2];
//              *ma=*ma+ac;
//              *mb=*mb+bc;
//              *ml=*ml+lc;
              //cerr << lc << ":" ;
              *ml=max((double)*ml,(double)lc);
              *ma=max((double)*ma,(double)ac);
              *mb=max((double)*mb,(double)bc);
          }
      }


//      *ma=pow((double)*ma,(double)1/p);
//      *mb=pow((double)*mb,(double)1/p);
//      *ml=pow((double)*ml,(double)1/p);


      double r=0;

      if(m==0)
      {
          r=(*ma+*mb+*ml)/3;

          *ma=r/(*ma);
          *mb=r/(*mb);
          *ml=r/(*ml);
          cerr <<  *ml << endl;
          cerr <<  *ma << endl;
          cerr <<  *mb << endl;

      }
      if(m==1)
      {
       r=(*ma+*mb+*ml)/3;
       r=max((double)*ma,(double)*mb);
       r=max((double)r,(double)*ml);

       *ma=r/(*ma);
       *mb=r/(*mb);
       *ml=r/(*ml);
       cerr <<  *ml << endl;
       cerr <<  *ma << endl;
       cerr <<  *mb << endl;

      }
      if(m==2)
      {
          r=sqrt((*ma)*(*ma)+(*mb)*(*mb)+(*ml)*(*ml));
         *ma=(double)(*ma)/(double)r;
          *mb=(double)(*mb)/(double)r;
          *ml=(double)(*ml)/(double)r;

          cerr <<  *ml << endl;
          cerr <<  *ma << endl;
          cerr <<  *mb << endl;

          r=max((double)*ma,(double)*mb);
          r=max((double)r,(double)*ml);

          *ma=sqrt((double)r/(double)(*ma));
          *mb=sqrt((double)r/(double)(*mb));
          *ml=sqrt((double)r/(double)(*ml));
      }




}








 
bool Buoy::detectBuoy()
{
    if(!_image.empty())
    {
    	Mat org,_finalImg;
    	org = _image;
    	cout << "org image is being displayed; "<< endl;

    	imshow("org" , _image);

    	imshow("maxRGB",b3.run(org,6,0));
		imshow("max edge",b4.run(org,1,0)); 



    	waitKey(33);
    	return 0;
    }
    return 0;
    //     cvtColor(_image, _imageHSV, CV_BGR2HSV);

    //     inRange(_imageHSV,_lowerThreshRed1,_upperThreshRed1, _imageBW);
    //     inRange(_imageHSV,_lowerThreshRed2,_upperThreshRed2, _imageBWRed);
    //     add(_imageBW, _imageBWRed, _imageBW);
        
    //     inRange(_imageHSV,_lowerThreshGreen,_upperThreshGreen, _imageBWGreen);
    //     add(_imageBW, _imageBWGreen, _imageBW);
    //     imshow("_imageBW", _imageBW);
    //     waitKey(33);

    //     medianBlur(_imageBW, _imageBW, 3);
    //     erode(_imageBW, _imageBW, _kernelDilateErode);
    //     CBlobResult _blobs,_blobsClutter;
    //     CBlob * _currentBlob;
    //     IplImage _imageBWipl = _imageBW;

    //     _blobs = CBlobResult(&_imageBWipl, NULL, 0);
    //     _blobs.Filter(_blobs, B_INCLUDE, CBlobGetArea(), B_INSIDE, 50, 1000);

    //     _imageBW = Scalar(0, 0, 0);
    //     for(int i = 0; i < _blobs.GetNumBlobs(); i++)
    //     {
    //         _currentBlob = _blobs.GetBlob(i);
    //         _currentBlob->FillBlob(&_imageBWipl, Scalar(255));
    //     }
        
    //     Mat _imageBW2 = _imageBW.clone();
    //     Mat src;
    //     vector<Mat> channels;
    //     channels.push_back(_imageBW);
    //     channels.push_back(_imageBW);
    //     channels.push_back(_imageBW);
    //     merge( channels, src);

    //     _contours.clear();
    //     medianBlur(_imageBW2, _imageBW2, 5);
    //     findContours(_imageBW2, _contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    //     Point2f _centerBuff;
    //     float _radiusBuff;
    //     vector<Point> _contoursPolyBuff;
    //     _center.clear();
    //     _radius.clear();
    //     _contoursPoly.clear();

    //     _imageBW = Scalar(0, 0, 0);

    //     for(int i=0; i < _contours.size(); i++)
    //     {
    //         if(contourArea(_contours[i])>50)
    //         {
    //            approxPolyDP(_contours[i],_contoursPolyBuff,3,true);
    //            minEnclosingCircle((Mat)_contoursPolyBuff,_centerBuff,_radiusBuff);
    //            circle(_imageBW,_centerBuff,_radiusBuff,Scalar(255), -1);
    //            _center.push_back(_centerBuff);
    //            _radius.push_back(_radiusBuff);
    //            _contoursPoly.push_back(_contoursPolyBuff);
    //         }
    //     }

        
    //     Mat src_gray;
    //     cvtColor( src, src_gray, CV_BGR2GRAY );
    //     src = Scalar(0, 0, 0);

    //     GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );
    //     imshow("src_gray", src_gray);

    //       vector<Vec3f> circles;

    //       /// Apply the Hough Transform to find the circles
    //       HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/16, 150, 25, 0, 0 );

    //       /// Draw the circles detected
    //       if(circles.size() == 0)
    //         cout<<"NOTHING CIRCULAR" << endl;

    //       for( size_t i = 0; i < circles.size(); i++ )
    //       {
    //           Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
    //           int radius = cvRound(circles[i][2]);
    //           // circle center
    //           circle( src, center, 3, Scalar(0, 255, 0), 3, 8, 0 );
    //           // circle outline
    //           circle( src, center, radius, Scalar(0, 0, 255), 1, 8, 0 );
    //        }

    //     imshow("src", src);

    //     if(_center.size() > 0)
    //         return true;
    //     else
    //         return false;
    // }
    // else
    //     return false;
}

void Buoy::getAllignment()
{
    for(int i = 0; i < _center.size(); i++)
    {
        _feedback.errorx = _image.cols/2 - _center[i].x;
        _feedback.errory = _image.rows/2 - _center[i].y;
        cout << "errorx and errory respectively : " ;
        cout<< _feedback.errorx << " and  " << _feedback.errory << endl;
    }
}



Buoy::~Buoy()
{

}

int main(int argc, char ** argv)
{
    ros::init(argc, argv, "buoy_server");
    Buoy _buoyserver("buoy");
    ros::spin();
    return 0;
}
