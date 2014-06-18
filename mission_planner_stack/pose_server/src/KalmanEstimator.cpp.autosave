#include "pose_server/KalmanEstimator.h"

using namespace kraken_core;

KalmanEstimator::KalmanEstimator(int size, float time):Estimator(size,time){


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

void KalmanEstimator::updatePose(kraken_msgs::imuData &imu_msg)
{
}

void KalmanEstimator::updatePose(kraken_msgs::imuData &imu_msg, kraken_msgs::depthData &depth_msg)
{
}

void KalmanEstimator::updatePose(kraken_msgs::imuData &imu_msg, kraken_msgs::depthData &dep_msg, kraken_msgs::dvlData &dvl_msg)
{
}

void KalmanEstimator::resetPose(KrakenPose &pose_msg)
{
}

Vector4d KalmanEstimator::getStateMatrix()
{
    Vector4d X;
    float* _currentPose=_prev_states_world.at(0).getData();
    X<<_currentPose[kraken_core::_px],_currentPose[kraken_core::_py],
            _currentPose[kraken_core::_vx],_currentPose[kraken_core::_vy];
    return X;
}

void KalmanEstimator::kalmanUpdateState(double ax, double ay)
{
    Vector2d U(ax,ay);
    Vector4d X=_Fmatrix*getStateMatrix()+_Bmatrix*U;
    _Pmatrix=_Fmatrix*_Pmatrix*_Fmatrix.transpose();
    updateStateFromMatrix(X);

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
    updateStateFromMatrix(temp);
    _Pmatrix=(Matrix4d::Identity()-K*_Hmatrix)*_Pmatrix;
}




void KalmanEstimator::updateStateFromMatrix(const Vector4d &stateVector)
{

}





