#include <ros/ros.h>
#include <task_buoy/buoy_server.h>
#define PI 3.1414

#include <resources/topicHeader.h>

#include <kraken_msgs/positionData.h>


/* white balance code starts here*/


class maxRGB
{
public:
    Mat  run(Mat,int p,int m);
    void  process(Mat src1,float *ml,float *ma,float *mb,int p,int m);
};

maxRGB obj;


Mat  maxRGB::run(Mat src1,int p,int m)
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


void maxRGB::process(Mat src1,float *ml,float *ma,float *mb,int p,int m)
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

Buoy::Buoy(std::string name) : _it(_n), _s(_n, name, boost::bind(&Buoy::executeCB, this, _1), false), _actionName(name)
{
    _sub = _it.subscribe(topics::CAMERA_FRONT_RAW_IMAGE, 1, &Buoy::imageCallBack, this);
    _pub = _it.advertise(topics::CAMERA_FRONT_BUOY_IMAGE, 1);

    _buoy_coord_pub = _nh.advertise<kraken_msgs::positionData>("/kraken/buoy/coordinates/", 1);

    max = 0;
    ifstream _thresholdVal("threshold.th");
    if(_thresholdVal.is_open())
    {
        for(int i = 0; i < 3; i++)
        {
            _thresholdVal >> _lowerThreshYellow[i];
        }
        for(int i = 0; i < 3; i++)
        {
            _thresholdVal >> _upperThreshYellow[i];
        }
        // for(int i = 0; i < 3; i++)
        // {
        //   _thresholdVal >> _lowerThreshRed2[i];
        // }
        // for(int i = 0; i < 3; i++)
        // {
        //   _thresholdVal >> _upperThreshRed2[i];
        // }
        /*for(int i = 0; i < 3; i++)
        {
            _thresholdVal >> _lowerThreshGreen[i];
        }
        for(int i = 0; i < 3; i++)
        {
            _thresholdVal >> _upperThreshGreen[i];
        }*/
    }
    else
    {
        ROS_ERROR("Unable to open threshold file.");
        ros::shutdown();
    }

    cout << "threshold files read " << endl;
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
            else
                cout << "buoy not detected... " << endl;
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
            /*if((_feedback.errorx < 10 && _feedback.errorx > -10) && (_feedback.errory > -10 && _feedback.errory < 10))
            {
                _result.sequence.push_back(BUOY_ALIGNED);
                cout << "<<<<<<<<<<<<  BUOY_ALIGNED   >>>>>>>>>>>>>>" << endl;
                break;
            }*/
            _s.publishFeedback(_feedback);
            looprate.sleep();
        }
        break;
    }
    }
    if(success)
    {
        _result.sequence.push_back(_goal->order);
        kraken_msgs::positionData temp; 
       // temp.x_coordinate = -1 * _feedback.errorx;
        //temp.y_coordinate = _feedback.errory;
        //_buoy_coord_pub.publish(temp);

        _s.setSucceeded(_result);
    }
}


bool Buoy::detectBuoy()
{
    if(!_image.empty())
    {
        Mat org,_finalImg, gray, edges,dst, org1;
        _finalImg = obj.run(_image,1,0);
//        imshow("max edge",obj.run(_image,1,0));
//        cvtColor(_finalImg, _imageHSV, CV_BGR2HSV);
//        Mat gray,dst;
	imshow("final image", _finalImg);
        cvtColor(_finalImg,gray,CV_BGR2GRAY);
        adaptiveThreshold(gray,org,100,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,13,0);

//        fastNlMeansDenoising(org,dst,3,7,21 );
//        fastNlMeansDenoising(org, dst);
//        imshow("denoised ", dst);
//        inRange(_imageHSV,_lowerThreshYellow,_upperThreshYellow, org);
        // inRange(_imageHSV,_lowerThreshGreen,_upperThreshGreen, _imageBWGreen);

//         imshow("org", org);

        Mat org2, erodeimg;
//        medianBlur(org, _imageBW, 3);

        erode(org, erodeimg, _kernelDilateErode);
        erode(erodeimg, erodeimg, _kernelDilateErode);
  //      imshow("erode", erodeimg);
        dilate(erodeimg, org2, _kernelDilateErode);
//        dilate(org2, org2, _kernelDilateErode);
        Canny(org2,edges,0,100);
//        imshow("eroded image", edges);
    //    imshow("dilated ", org2);
       // imshow("canny", edges);
        waitKey(33);
        CBlobResult _blobs,_blobsClutter;
        CBlob * _currentBlob;
        IplImage _imageBWipl = org2;
        _blobs = CBlobResult(&_imageBWipl, NULL, 0);
        _blobs.Filter(_blobs, B_INCLUDE, CBlobGetArea(), B_INSIDE, 200, 5000);

//         _imageBW = Scalar(0, 0, 0);
        org2 = Scalar(0, 0, 0);

        cout << "number of blobs " << _blobs.GetNumBlobs() << endl;

        for(int i = 0; i < _blobs.GetNumBlobs(); i++)
        {
            _currentBlob = _blobs.GetBlob(i);
            _currentBlob->FillBlob(&_imageBWipl, Scalar(255));
        }

        Mat newimg;
//        Mat _imageBW2 = _imageBW.clone();
        Mat _imageBW2 = org2.clone();
        dilate(_imageBW2, newimg, _kernelDilateErode);
      //  imshow("newimg", newimg);
        Mat src;
        vector<Mat> channels;
        channels.push_back(org2);
        channels.push_back(org2);
        channels.push_back(org2);
        merge( channels, src);

        _contours.clear();
//        medianBlur(_imageBW2, _imageBW2, 5);
       // imshow("imageBW2", _imageBW2);
        findContours(_imageBW2, _contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

        Point2f _centerBuff;
        float _radiusBuff;
        vector<Point> _contoursPolyBuff;
        _center.clear();
        _radius.clear();
        _contoursPoly.clear();

        _imageBW = Scalar(0, 0, 0);

        for(int i=0; i < _contours.size(); i++)
        {
            if(contourArea(_contours[i])>50)
            {
                approxPolyDP(_contours[i],_contoursPolyBuff,3,true);
                minEnclosingCircle((Mat)_contoursPolyBuff,_centerBuff,_radiusBuff);
                circle(_imageBW,_centerBuff,_radiusBuff,Scalar(255), -1);
                _center.push_back(_centerBuff);
                _radius.push_back(_radiusBuff);
                _contoursPoly.push_back(_contoursPolyBuff);
            }
        }

        Mat src_gray;
        cvtColor( src, src_gray, CV_BGR2GRAY );
        src = Scalar(0, 0, 0);

        GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );
       // imshow("src_gray", src_gray);

        /// Apply the Hough Transform to find the circles
        HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, 80, 180, 25, 30, 300 );

        //       /// Draw the circles detected
        if(circles.size() == 0){
            cout<<"NOTHING CIRCULAR" << endl;
            return false;
        }





        for( size_t i = 0; i < circles.size(); i++ )
        {
            cout << "circle wala for loop " << endl;
            cout << "circle area = " << endl;
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            _fcenter.push_back(center);
            _fradius.push_back(cvRound(circles[i][2]));
            // circle center
            circle( src, center, 3, Scalar(0, 255, 0), 3, 8, 0 );
            // circle outline
            circle( src, center, radius, Scalar(0, 0, 255), 1, 8, 0 );
        }

        
        for (int i = 0; i < _fradius.size(); ++i)
        {
            if(_fradius[i] > max){
                index = i;
                max = _fradius[i];
            }
        }
        cout << "<<<<<<<<<<     largest radius = " << max << "          >>>>>>>>>>>>>>>"<<endl;
        if(max == 0){
            cout << "max  = 0" << endl;
            return false;
        }
        else{
            circle(src,_fcenter[index],3,Scalar(255,255,0),1,8,0);
            circle(src,_fcenter[index], max,Scalar(0,255,0),1,8,0);
//            return true;
        }
        cout << "show src image" << endl;
        imshow("src", src);

        _fcenter.clear();
        _fradius.clear();
//        if(_center.size() > 0){
//            cout << "center size > 0 " << endl;
//            return true;
//        }
//        else{
//            cout << "returns false" << endl;
//            return false;
//        }

        return true;
    }
    else{
        cout << "no image loaded.." << endl;
        return false;
    }


    waitKey(33);
    return 0;
}


void Buoy::getAllignment()
{
	cout<<_center.size()<<endl;
 //   for(int i = 0; i < _center.size(); i++)
  //  {
        _feedback.errorx =  _center[index].x-_image.cols/2;
        _feedback.errory = _image.rows/2 - _center[index].y;
	/*if (i>=1)
	{
		_feedback.errory=_feedback.errory<=_center[i].y?_feedback.errory:_image.rows/2 -_center[i].y;
		_feedback.errorx=_feedback.errory<=_center[i].y?_feedback.errorx:_image.cols/2 -_center[i].x;
	}
    }*/
        cout << "errorx and errory respectively : " ;
        cout<< _feedback.errorx << " and  " << _feedback.errory << endl;
	
	kraken_msgs::positionData temp; 
        temp.x_coordinate = _feedback.errorx;
        temp.y_coordinate = _feedback.errory;
        _buoy_coord_pub.publish(temp);

}

Buoy::~Buoy()
{
}

int main(int argc, char ** argv)
{
    ros::init(argc, argv, "buoy_server");
    cout << "running main " <<endl;
    Buoy _buoyserver("buoy");
    cout << "node made.. " << endl;
    ros::spin();
    return 0;
}
