#ifndef _POSE_SERVER_KALMAN_H
#define _POSE_SERVER_KALMAN_H

#include "eigen3/Eigen/Dense"
#include "eigen3/Eigen/Geometry"
#include <iostream>
#include <pose_server/Estimator.h>
using namespace Eigen;
namespace kraken_core{

    class KalmanEstimator:public Estimator{
    public:
        KalmanEstimator(int size=2,float time=0.05);
        virtual void updatePose(kraken_msgs::imuData &);
        virtual void updatePose(kraken_msgs::imuData &,kraken_msgs::depthData&);
        virtual void updatePose(kraken_msgs::imuData &,kraken_msgs::depthData&,kraken_msgs::dvlData&);
        virtual void resetPose(KrakenPose & pose_msg);
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


        void updateStateFromMatrix(const Vector4d &stateVector);
        Vector4d getStateMatrix();


    };

}


#endif
