#include "pose_server/KalmanEstimator.h"

using namespace kraken_core;

KalmanEstimator::KalmanEstimator(int size, float time):Estimator(size,time),_isStateInitiaized(false){


    //defining all the 4 matrices;
    _Hmatrix<<0,0,1,0,
               0,0,0,1;

    _Fmatrix<<1,0,_time,0,
            0,1,0,_time,
            0,0,1,0,
            0,0,0,1;

    _Pmatrix<<0.05,0,0,0,
            0,0.05,0,0,
            0,0,0.01,0,
            0,0,0,0.01;

    _Rmatrix<<0.01,0,
            0,0.01;

    _Bmatrix<<_time*_time,0,
            0,_time*_time,
            _time,0,
            0,_time;

}

/*void KalmanEstimator::initalizeState()
{
    float* data=_next_pose_world.getData();
    data[kraken_core::_px]=0;
    data[kraken_core::_py]=0;
    data[kraken_core::_vx]=0;
    data[kraken_core::_vy]=0;
    data[kraken_core::_ax]=0;
    data[kraken_core::_ay]=0;
    data[kraken_core::_az]=0;
}*/


void KalmanEstimator::updatePose(kraken_msgs::imuData &imu_msg)
{
    updateState(imu_msg);
    float* world_data=_next_pose_world.getData();
    kalmanUpdateState(world_data[kraken_core::_ax],world_data[kraken_core::_ay]);
    updateZvalues(world_data[kraken_core::_vz],world_data[kraken_core::_az]);
}

void KalmanEstimator::updatePose(kraken_msgs::imuData &imu_msg, kraken_msgs::depthData &depth_msg)
{
    updateState(imu_msg);
    updateState(depth_msg);
    float* world_data=_next_pose_world.getData();
    kalmanUpdateState(world_data[kraken_core::_ax],world_data[kraken_core::_ay]);
}

void KalmanEstimator::updatePose(kraken_msgs::imuData &imu_msg, kraken_msgs::depthData &dep_msg, kraken_msgs::dvlData &dvl_msg)
{
    updateState(imu_msg);
    updateState(dep_msg);
    updateState(dvl_msg);
    float* world_data=_next_pose_world.getData();
    kalmanUpdateState(world_data[kraken_core::_ax],world_data[kraken_core::_ay]);
    kalmanMeasurementUpdate(world_data[kraken_core::_vx],world_data[kraken_core::_vy]);

}

void KalmanEstimator::resetPose(KrakenPose &pose_msg)
{
   return;
}


void KalmanEstimator::kalmanUpdateState(double ax, double ay)
{
    Vector2d U(ax,ay);
    Vector4d Xi=getStateMatrix();
    std::cout<<"Xi\n"<<Xi<<std::endl;
    Vector4d X=_Fmatrix*Xi+_Bmatrix*U;
    std::cout<<"Xf\n"<<X<<std::endl;
    _Pmatrix=_Fmatrix*_Pmatrix*_Fmatrix.transpose();
    std::cout<<_Pmatrix<<std::endl;
    kalmanUpdateStateFromMatrix(X);

}

void KalmanEstimator::kalmanMeasurementUpdate(double vx, double vy)
{
    Vector2d Z(vx,vy);
    Vector2d y;
    Vector4d X;
    X=getStateMatrix();


    y=Z-_Hmatrix*X;
    Matrix2d S=_Hmatrix*_Pmatrix*_Hmatrix.transpose()+ _Rmatrix;

    //assuming S is invertible
    Matrix<double,4,2> K=_Pmatrix*_Hmatrix.transpose()*S.inverse();


    Vector4d temp=X+K*y;
    kalmanUpdateStateFromMatrix(temp);
    _Pmatrix=(Matrix4d::Identity()-K*_Hmatrix)*_Pmatrix;
}




void KalmanEstimator::kalmanUpdateStateFromMatrix(const Vector4d &stateVector)
{
    float *current_state_array=_next_pose_world.getData();
    current_state_array[kraken_core::_px]=stateVector(0);
    current_state_array[kraken_core::_py]=stateVector(1);
    current_state_array[kraken_core::_vx]=stateVector(2);
    current_state_array[kraken_core::_vy]=stateVector(3);
}

void KalmanEstimator::updateState(kraken_msgs::imuData &imu)
{
    //should be called at first for updating all the required imu values
    /*
    float *current_state_array=_next_pose_world.getData();

    current_state_array[kraken_core::_ax]=imu.data[kraken_sensors::accelX];
    current_state_array[kraken_core::_ay]=imu.data[kraken_sensors::accelY];
    current_state_array[kraken_core::_az]=imu.data[kraken_sensors::accelZ];
    current_state_array[_w_roll]   =  imu.data[kraken_sensors::gyroX];
    current_state_array[_w_pitch]  =  imu.data[kraken_sensors::gyroY];
    current_state_array[_w_yaw]    =  imu.data[kraken_sensors::gyroZ];

    current_state_array[_roll]   = imu.data[kraken_sensors::roll];
    current_state_array[_pitch]  = imu.data[kraken_sensors::pitch];
    current_state_array[_yaw]    = imu.data[kraken_sensors::yaw];*/


    float* _data_body_next  = _next_pose_body.getData();
    float* _data_world_next = _next_pose_world.getData();
    // Update body accelaration buffer
    _data_body_next[_ax] = imu.data[kraken_sensors::accelX];
    _data_body_next[_ay] = imu.data[kraken_sensors::accelY];
    _data_body_next[_az] = imu.data[kraken_sensors::accelZ];
    // Update body angular velocity buffer
    _data_world_next[_w_roll]   = _data_body_next[_w_roll]  = imu.data[kraken_sensors::gyroX];
    _data_world_next[_w_pitch]  = _data_body_next[_w_pitch] = imu.data[kraken_sensors::gyroY];
    _data_world_next[_w_yaw]    = _data_body_next[_w_yaw]   = imu.data[kraken_sensors::gyroZ];
    // Update body angular position buffer
    _data_world_next[_roll]   = _data_body_next[_roll]  = imu.data[kraken_sensors::roll];
    _data_world_next[_pitch]  = _data_body_next[_pitch] = imu.data[kraken_sensors::pitch];
    _data_world_next[_yaw]    = _data_body_next[_yaw]   = imu.data[kraken_sensors::yaw];
    accelerationToWorld();
}

void KalmanEstimator::updateState(kraken_msgs::depthData &depth_data_msg)
{
    float *world_data=_next_pose_world.getData();
    world_data[kraken_core::_vz]=(depth_data_msg.depth-world_data[kraken_core::_pz])/_time;
    world_data[kraken_core::_pz]=depth_data_msg.depth;
}

void KalmanEstimator::updateState(kraken_msgs::dvlData& dvl_data)
{
    float* body_data=_next_pose_body.getData();
//    world_data[kraken_core::_pz]=dvl_data.data[kraken_sensors::_dvl_vz];
    body_data[kraken_core::_vx]=dvl_data.data[kraken_sensors::_dvl_vx];
    body_data[kraken_core::_vy]=dvl_data.data[kraken_sensors::_dvl_vy];
    body_data[kraken_core::_vz]=dvl_data.data[kraken_sensors::_dvl_vz];
    velocityToWorld();
}



void KalmanEstimator::accelerationToWorld()
{
    //gets acceleration in world frame in _next_pose_world
    float* world_data=_next_pose_world.getData();
    float *body_data=_next_pose_body.getData();


    float ax=body_data[kraken_core::_ax];
    float ay=body_data[kraken_core::_ay];
    float az=body_data[kraken_core::_az];
    float r=body_data[kraken_core::_roll];
    float p=body_data[kraken_core::_pitch];
    float y=body_data[kraken_core::_yaw];


    Matrix3d RWB;
    RWB<<cos(p)*cos(y),     sin(r)*sin(p)*cos(y)-cos(r)*sin(y),            cos(r)*sin(p)*cos(y)+sin(r)*sin(y),
            cos(p)*sin(y),   sin(r)*sin(p)*sin(y)+cos(r)*cos(y),           cos(r)*sin(p)*sin(y)-sin(r)*cos(y),
            -sin(p)  ,        sin(r)*cos(y),                                 cos(r)*cos(p);

    Vector3d Aw=RWB*Vector3d(ax,ay,az);

    world_data[kraken_core::_ax]=Aw(0);
    world_data[kraken_core::_ay]=Aw(1);
    world_data[kraken_core::_az]=Aw(2);

}


void KalmanEstimator::velocityToWorld()
{
    //gets velocity in world frame in _next_pose_world
    float* world_data=_next_pose_world.getData();
    float *body_data=_next_pose_body.getData();


    float vx=body_data[kraken_core::_vx];
    float vy=body_data[kraken_core::_vy];
    float vz=body_data[kraken_core::_vz];
    float r=body_data[kraken_core::_roll];
    float p=body_data[kraken_core::_pitch];
    float y=body_data[kraken_core::_yaw];


    Matrix3d RWB;
    RWB<<cos(p)*cos(y),     sin(r)*sin(p)*cos(y)-cos(r)*sin(y),            cos(r)*sin(p)*cos(y)+sin(r)*sin(y),
            cos(p)*sin(y),   sin(r)*sin(p)*sin(y)+cos(r)*cos(y),           cos(r)*sin(p)*sin(y)-sin(r)*cos(y),
            -sin(p)  ,        sin(r)*cos(y),                                 cos(r)*cos(p);

    Vector3d Vw=RWB*Vector3d(vx,vy,vz);

    world_data[kraken_core::_vx]=Vw(0);
    world_data[kraken_core::_vy]=Vw(1);
    world_data[kraken_core::_vz]=Vw(2);

}





