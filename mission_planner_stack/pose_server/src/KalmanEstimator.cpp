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

void KalmanEstimator::initalizeState()
{
    float* data=_current_state_world.getData();
    data[kraken_core::_px]=0;
    data[kraken_core::_py]=0;
    data[kraken_core::_vx]=0;
    data[kraken_core::_vy]=0;
    data[kraken_core::_ax]=0;
    data[kraken_core::_ay]=0;
    data[kraken_core::_az]=0;
}


void KalmanEstimator::updatePose(kraken_msgs::imuData &imu_msg)
{
    updateState(imu_msg);
    kalmanUpdateState(imu_msg.data[kraken_core::_ax],imu_msg.data[kraken_core::_ay]);
    float *current_state_array=_next_pose_world.getData();
    updateZvalues(current_state_array[kraken_core::_az],current_state_array[kraken_core::_vz]);
}

void KalmanEstimator::updatePose(kraken_msgs::imuData &imu_msg, kraken_msgs::depthData &depth_msg)
{
    updateState(imu_msg);
    updateState(depth_msg);
    kalmanUpdateState(imu_msg.data[kraken_core::_ax],imu_msg.data[kraken_core::_ay]);
}

void KalmanEstimator::updatePose(kraken_msgs::imuData &imu_msg, kraken_msgs::depthData &dep_msg, kraken_msgs::dvlData &dvl_msg)
{
    updateState(imu_msg);
    updateState(dep_msg);
    updateState(dvl_msg);
    kalmanUpdateState(imu_msg.data[kraken_core::_ax],imu_msg.data[kraken_core::_ay]);
    kalmanmeasurementUpdate(dvl_msg.data[kraken_sensors::_dvl_vx],dvl_msg.data[kraken_sensors::_dvl_vy]);

}

void KalmanEstimator::resetPose(KrakenPose &pose_msg)
{
   return;
}


void KalmanEstimator::kalmanUpdateState(double ax, double ay)
{
    Vector2d U(ax,ay);
    Vector4d X=_Fmatrix*getStateMatrix()+_Bmatrix*U;
    _Pmatrix=_Fmatrix*_Pmatrix*_Fmatrix.transpose();
    kalmanUpdateStateFromMatrix(X);

}

void KalmanEstimator::kalmanmeasurementUpdate(double vx, double vy)
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

    float *current_state_array=_next_pose_world.getData();

    current_state_array[kraken_core::_ax]=imu.data[kraken_sensors::accelX];
    current_state_array[kraken_core::_ay]=imu.data[kraken_sensors::accelY];
    current_state_array[kraken_core::_az]=imu.data[kraken_sensors::accelZ];
    current_state_array[_w_roll]   =  imu.data[kraken_sensors::gyroX];
    current_state_array[_w_pitch]  =  imu.data[kraken_sensors::gyroY];
    current_state_array[_w_yaw]    =  imu.data[kraken_sensors::gyroZ];

    current_state_array[_roll]   = imu.data[kraken_sensors::roll];
    current_state_array[_pitch]  = imu.data[kraken_sensors::pitch];
    current_state_array[_yaw]    = imu.data[kraken_sensors::yaw];
}

void KalmanEstimator::updateState(kraken_msgs::depthData &depth_data_msg)
{
    float *current_state_array=_next_pose_world.getData();
    current_state_array[kraken_core::_vz]=(depth_data_msg.depth-current_state_array[kraken_core::_pz])/_time;
    current_state_array[kraken_core::_pz]=depth_data_msg.depth;
}

void KalmanEstimator::updateState(kraken_msgs::dvlData& dvl_data)
{
    float *current_state_array=_next_pose_world.getData();
    current_state_array[kraken_core::_pz]=dvl_data.data[kraken_sensors::_dvl_vz];
}

void KalmanEstimator::updateCurrentAccelaration(kraken_msgs::imuData & imu)
{
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
}






