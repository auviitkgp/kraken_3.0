#ifndef VALIDATION_GATE_cpp
#define VALIDATION_GATE_cpp

#include <vgtest/vgheader.h>
#include <vgtest/vgGoal.h>
#include <vgtest/vgResult.h>

void printpoints(const vector<Point> &v)
{
    printf("\n");
    for(size_t i=0;i<v.size();i++)
        printf("(%d %d)",v[i].x,v[i].y);
    printf("\n");
}

bool sort_lower(Point x,Point y)//sort according to  y-cordinate
{
    if(x.y>y.y)
        return false;
    return true;
}

bool sort_left(Point a,Point b)
{
    if(a.x>b.x)
        return false;
    return true;
}

Gateserver::Gateserver(NodeHandle &n1)
    :_n(n1),_it(_n),
      _serv(_n,"gate_server",boost::bind(&Gateserver::serverCallback,this,_1),false)
{

    _imgsub = _it.subscribe(topics::CAMERA_FRONT_RAW_IMAGE,10,&Gateserver::imageCallback,this);
    cout << "subscribed from topic = " << topics::CAMERA_FRONT_RAW_IMAGE << endl;
    _epsilon=1;
    _THRES_LINE_GAP = 15;
    index = 0;
    _kernel = getStructuringElement(MORPH_RECT, Size(3,3),Point(-1,-1));
    _pub = _it.advertise("validationGate",1);
    cog.x = _img.rows/2;
    cog.y = _img.cols/2;
    prevmed.x = 0;
    prevmed.y = 0;
    _serv.start();
    ROS_INFO("waiting for clients");
}


void Gateserver::printbunch(vector<bunch> &b, string name){
    Mat img(500, 500, CV_8UC3, Scalar(0,0,255));
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



inline bool Gateserver::withinThres(int th,int a ,int b)
{
    if(fabs(b-a)<th)
        return true;
    else
        return false;
}

pair<bunch,pair<bunch,bunch> > Gateserver::getLargestBunch(const vector<Vec4i> &lines){
    vector<bunch> bunches_x,bunches_y;
    for (int i = 0; i < lines.size(); ++i) {
        double theta;
        if(lines[i][2]-lines[i][0]!=0)
            theta = atan((lines[i][3]-lines[i][1])/(lines[i][2]-lines[i][0]));
        else theta = 90*CV_PI/180;
        if(theta>CV_PI/180*-30&&theta<30*CV_PI/180)
        {
            Point right;
            if(lines[i][0]<lines[i][2])
                right.x=lines[i][2],right.y=lines[i][3];
            else
                right.x=lines[i][0],right.y=lines[i][1];
            bool added=false;
            for (int j = 0; j <bunches_x.size() ; ++j) {

                ///! bunches_x is an empty array... did not push bunch in that...


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

Point Gateserver::getCenterOfGate()
{
    Point zero;
    zero.x =0;
    zero.y =0;

//    cout << "cof called..\n";
    cof = getCenterOfFrame();
//    cout << "cof done..\n";
    if(cof.x==0&& cof.y==0)
        return prevmed;
    if(q.size()<10)
    {
        q.push_back(cof);
        vector<Point> nomove(q.begin(), q.end());
        prevmed = getMeanOfModes(nomove);
        return zero;
    }
    else{
        if(withinThres(30,cof.x,prevmed.x) && withinThres(30,cof.y,prevmed.y))
        {
            q.pop_front();
            q.push_back(cof);
            vector<Point> newq(q.begin(),q.end());
            cog = getMeanOfModes(newq);
            prevmed=cog;
            return cog;
        }else{
            return prevmed;
        }
    }

}

Point Gateserver::getCenterOfFrame()
{
//    cout << "getcof..\n";
    cvtColor(_img,_hsv,CV_BGR2GRAY);
    //       inRange(_hsv,_low,_high,_thresh);
    //       medianBlur(_thresh,_median,3);
    adaptiveThreshold(_hsv,_median,200,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,13,0);
//    imshow("adaptive", _median);
    erode(_median,_dilateImg,_kernel);
    dilate(_dilateImg,_dilateImg,_kernel);
//    Point median ;
//    median.x = _dilateImg.rows/2;
//    median.y = _dilateImg.cols/2;
//    imshow("erosion",_dilateImg);
//    Canny(_dilateImg,_canny,20,200,3);
//    cout << "2 \n";
    IplImage fimage = _dilateImg;
    _blobs1 = CBlobResult(&fimage,NULL,0);
    _blobs2 = CBlobResult(&fimage,NULL,0);
//    cout << "blobs.. \n";
    _blobs1.Filter(_blobs1, B_EXCLUDE, CBlobGetArea(), B_GREATER, 100);
    _blobs2.Filter(_blobs2,B_INCLUDE,CBlobGetArea(), B_LESS, 40);

//    cout << "1";
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

    HoughLinesP(fmat,_lines,1,CV_PI/180,60,30,10);

//    cout << "houghlines \n";
    retpair = getLargestBunch(_lines);
    b=retpair.first;
    vector<Vec4i> &v = b.v;
    for(int i=0; i<v.size(); i++){
        line(_img,Point(v[i][0],v[i][1]),Point(v[i][2],v[i][3]),Scalar(0,255,0));
    }

    if(retpair.second.first.v.size() == 0 || retpair.second.second.v.size() == 0 ){
        _status =  false;
        return Point(0,0);
    }else
        _status = true;

    bunch &hor = retpair.second.first, &ver=retpair.second.second;
    std::vector<Point> lowerpoints,leftpoints;


//    cout << "ver.v.size = " << ver.v.size() << endl;
//    cout << "hor.v.size = " << hor.v.size() << endl;
    for(int i=0; i<ver.v.size(); i++){
        if(ver.v[i][1]>ver.v[i][3]){
            Point pt = Point(ver.v[i][2],ver.v[i][3]);
//            cout << pt.x << " " << pt.y << endl;
            lowerpoints.push_back(Point(ver.v[i][2],ver.v[i][3]));
        }
        else{
            Point pt = Point(ver.v[i][0],ver.v[i][1]);
//            cout << pt.x << " " <<pt.y << endl;
            lowerpoints.push_back(Point(ver.v[i][0],ver.v[i][1]));
        }
    }

    for(int i=0; i<hor.v.size(); i++){
        if(hor.v[i][0]>hor.v[i][2])
            leftpoints.push_back(Point(hor.v[i][2],hor.v[i][3]));
        else
            leftpoints.push_back(Point(hor.v[i][0],hor.v[i][1]));
    }

    ROS_INFO("Lower points");
    printpoints(lowerpoints);

    ROS_INFO("left points");
    printpoints(leftpoints);

    sort(lowerpoints.begin(),lowerpoints.end(),sort_lower);
    sort(leftpoints.begin(),leftpoints.end(),sort_left);
    Point lower=getMeanOfModes(lowerpoints);
    Point left=getMeanOfModes(leftpoints);
    Point median((lower.x+left.x)/2,(lower.y+left.y)/2);
    return median;
}

bool Gateserver::detectGate()
{
    ROS_INFO("detectgate func entered..!!\n");
    _center = getCenterOfGate();
    if(_center.x == 0 && _center.y == 0)
        _status = false;
    else
        _status = true;
    return _status;
}


void Gateserver::alignGate()
{
    ROS_INFO("center.x and center.y are : %f %f ", _center.x, _center.y);
    _error.x  = _center.x-(_img.rows/2);
    _error.y  = _center.y-(_img.cols/2);

    imshow("original", _img);
    if(waitKey(33) ==27)
        return;
}

//execute callback function

void Gateserver::serverCallback(const ip_msgs::vgateGoalConstPtr &goal)
{
//    cout << "servercall back..\n";
    Rate looprate(10);
    switch(goal->gate)
    {
    case DETECT_GATE:
    {
//        cout << "start detecting validation gate...";
        while(ok())
        {
            if (_serv.isPreemptRequested())
            {
                _res.resultx= cog.x ;
                _res.resulty= cog.y;
                _serv.setPreempted(_res);
                break;
            }
            _gateStatus = detectGate();
            if(_gateStatus){
                ROS_INFO("gate has been detected.. ");
                _res.resultx = cog.x;
                _res.resulty = cog.y;
                _res.fresult = GATE_DETECTED;
                _serv.setSucceeded(_res);
            }else{
                ROS_INFO("gate has not been detected..\n continue search");
                _res.fresult = NOT_DETECTED;
                _serv.setAborted(_res);
            }
            _feed.xfeed = _center.x;
            _feed.yfeed = _center.y;
            _serv.publishFeedback(_feed);
            looprate.sleep();
        }
        break;

    }
    case ALIGN_GATE:
    {
        ROS_INFO("start aligning ...");
        while(ok())
        {
            if (_serv.isPreemptRequested())
            {
                _res.resultx = _center.x;
                _res.resulty = _center.y;
                _serv.setPreempted(_res);
                break;
            }

            _gateStatus = detectGate();
            if(_gateStatus){
                alignGate();

                if(_error.x <-20){
                    cout << "move to right" << endl;
                    _res.fresult = MOVE_TO_LEFT;
                }
                else if(_error.x > 20){
                    cout << "move to left" << endl;
                    _res.fresult = MOVE_TO_RIGHT;
                }
                if(_error.y >20&& _error.y>0 ){
                    cout << "move upwards" << endl;
                    _res.fresult = MOVE_UPWARDS;
                }
                else if(_error.y <-20){
                    cout << "move downwards.." << endl;
                    _res.fresult = MOVE_DOWNWARDS;
                }
                _feed.xfeed = _error.x;
                _feed.yfeed = _error.y;
                _serv.setSucceeded(_res);
            }else{
                _feed.xfeed = 0.0;
                _feed.yfeed = 0.0;
                _res.fresult = NOT_DETECTED;
                _serv.setAborted(_res);
            }
            _serv.publishFeedback(_feed);
            looprate.sleep();
        }
        break;
    }
    }
}



void Gateserver::imageCallback(const sensor_msgs::ImageConstPtr& ptr)
{
//    cout << "image call_back" << endl;
    try
    {
        bridge_ptr = cv_bridge::toCvCopy(ptr,"8UC3");
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }
    I2 = bridge_ptr->image;
    _img = I2;
    imshow("img", _img);
    waitKey(40);
}



std::vector<Point> getModePoints(vector<Point>& v)
{
    int maxm=0;
    int thresmap[100][100]={{0}};
    std::vector<Point> retvector;
    Point mode;
    for (int i = 0; i < v.size(); ++i)
    {

        thresmap[v[i].x/15][v[i].y/15]++;
        int prevmax=maxm;
        maxm=max(maxm,thresmap[v[i].x/15][v[i].y/15]);
        if(prevmax!=maxm){
            mode.x=v[i].x/15,mode.y=v[i].y/15;
        }

    }

    for (int i = 0; i < v.size(); ++i)
    {
        if(v[i].x/15==mode.x&&v[i].y/15==mode.y){
            retvector.push_back(v[i]);
        }
    }

    return retvector;
}


Point getMeanOfModes(vector<Point>& v)
{
    ROS_INFO("For points ");
    printpoints(v);
    std::vector<Point> modepoints=getModePoints(v);
    Point mean;
    mean.x=0,mean.y=0;
    ROS_INFO("Mode points");
    printpoints(modepoints);
    for (int i = 0; i < modepoints.size(); ++i)
    {
        mean.x+=modepoints[i].x;
        mean.y+=modepoints[i].y;
    }
    mean.x/=modepoints.size();
    mean.y/=modepoints.size();
    ROS_INFO("Mean of modes=%d %d",mean.x,mean.y);
    return mean;

}


Gateserver::~Gateserver()
{
}


int main(int argc, char** argv){
//    XInitThreads();
    ros::init(argc, argv, "server_node");
    //    if(argc <2){
    //        cout << "give the path of d video also..";
    //        return 0;
    //    }
//    cout << "main...";
    NodeHandle n;
    Gateserver gs(n);
    ros::spin();
    return 0;
}


#endif VALIDATION_GATE_cpp
