#ifndef _POSE_SERVER_KALMAN_H
#define _POSE_SERVER_KALMAN_H

#include "eigen3/Eigen/Dense"
#include "eigen3/Eigen/Geometry"
#include <iostream>
#include "pose_server/Estimator.h"
#include "NqDVL/NqDVL.h"
#include "tracks_imu/Tracks.h"

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




        void kalmanUpdateState(double ax,double ay);
        void kalmanmeasurementUpdate(double vx,double vy);


        void kalmanUpdateStateFromMatrix(const Vector4d &stateVector);

        inline Vector4d getStateMatrix(){
            Vector4d X;
            float* _currentPose=_next_pose_world.getData();
            X<<_currentPose[kraken_core::_px],_currentPose[kraken_core::_py],
                    _currentPose[kraken_core::_vx],_currentPose[kraken_core::_vy];
            return X;
        }


        inline void updateZvalues(float vz,float az){
            float *data=_next_pose_world.getData();
            data[kraken_core::_vz]=az*_time;
            data[kraken_core::_pz]=vz*_time+az*_time*_time*0.5;
        }

        void updateState(kraken_msgs::imuData &imu);
        void updateState(kraken_msgs::depthData &depth_data_msg);
        void updateState(kraken_msgs::dvlData&);
        void updateCurrentAccelaration(kraken_msgs::imuData &imu);
        KrakenPose _current_state_world;

        void initalizeState();

        bool _isStateInitiaized;
    };

}


#endif
