#ifndef _POSE_SERVER_KALMAN_H
#define _POSE_SERVER_KALMAN_H

#include "eigen3/Eigen/Dense"
#include "eigen3/Eigen/Geometry"
#include <iostream>
#include "pose_server/Estimator.h"
#include "pose_server/DeadReckoning.h"
#include "NqDVL/NqDVL.h"
#include "tracks_imu/Tracks.h"


/*
This class implements the kalmanFilter to estimate the AUV pose and then publishes it
It is assumed that imu,dvl data's it subscribes to are wrt the vehicle and it calcutes and publishes pose wrt earth
*/
using namespace Eigen;
namespace kraken_core{

    class KalmanEstimator:public Estimator{
    public:
        KalmanEstimator(int size=2,float time=0.05);
        virtual void updatePose(kraken_msgs::imuData &);
        virtual void updatePose(kraken_msgs::imuData &,kraken_msgs::depthData&);
        virtual void updatePose(kraken_msgs::imuData &,kraken_msgs::depthData&,kraken_msgs::dvlData&);
        virtual void resetPose(KrakenPose & pose_msg);
        ~KalmanEstimator(){}
    private:
        /**

        **/
        Matrix4d _Fmatrix;
        Matrix4d _Pmatrix;
        Matrix<double,2,4> _Hmatrix;
        Matrix2d _Rmatrix;
        Matrix<double,4,2> _Bmatrix;

        /*
          The state we assume here is X(t)=(x,y,vx,vy)
          F-- state transition matrix
          P-- covariance Matrix

          The predict step in kalman filter.
          X(t)=X(t-1)*F+ U
          P(t)=F*P(t-1)*F.transpose
        */
        void kalmanUpdateState(double ax,double ay);

        /*
          Z-- measurement
          H-- measurement function
          R-- measurement noise


          Kalman Measurement step (measurement is velocity from dvl)
          Y=Z-H*X
          S=H*P*H.tran+R
          K=P*H.tran*S.inv
          X=X+K*Y
          P=(I-K*H)*P
        */
        void kalmanMeasurementUpdate(double vx,double vy);
        void kalmanUpdateStateFromMatrix(const Vector4d &stateVector);

        inline Vector4d getStateMatrix(){
            Vector4d X;
            float* _currentPose=_next_pose_world.getData();
            X<<_currentPose[kraken_core::_px],_currentPose[kraken_core::_py],
                    _currentPose[kraken_core::_vx],_currentPose[kraken_core::_vy];
            return X;
        }


        inline void updateZvalues(float vz,float az){
            float *world_data=_next_pose_world.getData();
            world_data[kraken_core::_vz]+=az*_time;
            world_data[kraken_core::_pz]+=vz*_time+az*_time*_time*0.5;
        }


        void updateState(kraken_msgs::imuData &imu);
        void updateState(kraken_msgs::depthData &depth_data_msg);
        void updateState(kraken_msgs::dvlData&);



        void accelerationToWorld();
        void velocityToWorld();
        KrakenPose _current_state_world;



        bool _isStateInitiaized;
    };

}


#endif
