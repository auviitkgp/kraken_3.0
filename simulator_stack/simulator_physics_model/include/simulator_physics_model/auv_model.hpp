#ifndef _AUV_MODEL_H
#define _AUV_MODEL_H
#include <math.h>
#include <cstdlib>
#include <iostream>
namespace kraken_simulator{

class AuvModelSimple6DoF
{
    public:
        AuvModelSimple6DoF(float time);
        ~AuvModelSimple6DoF();
        float* _current_velocity_state_to_world;
        float* _previous_velocity_state_to_world;
        float _current_velocity_state_to_body[6];
        float _current_position_to_world[6];
        float _current_position_to_body[6];
        float _cos_rpy[3];
        float _sin_rpy[3];
        float _current_linear_velocity_body_to_world_matrix[3][3];
        float _current_angular_velocity_body_to_world_matrix[3][3];
        float _current_accelaration_to_body[6];
        float _current_accelaration_to_world[6];
        float _external_torque[6];
        float _damping_torque[6];
        /*
         * Force Defination
         * force[0] = forward thruster on left side, positive value takes vehicle forwards
         * force[1] = forward thruster on right side, positive value takes vehicle forwards
         * force[2] = sway thruster on front side, positive value takes vehicle rightwards
         * force[3] = sway thruster on back side, positive value takes vehicle rightwards
         * force[4] = depth thruster on back side, positive value takes vehicle downwards
         * force[5] = depth thruster on front side, positive value takes vehicle downwards
        */
        void updateAuv(float force[6]);
    private:
        float _time;
        /***** matrix thrust to force/torque ****/
        static const float L[6][6];
        /***** Gravity matrix ****/
        static const float G[6] ;
        /***** Mass matrix ****/
        static const float mass[6][6];
        /***** Damping matrix ****/
        static const float D[6][6];
        /************************************/
        static const double pi;
        /* initialize the vehicle parameters*/
        void initAuv();
        void updateCurrentPosition();
        void updateCurrentVelocity();
        void updateCurrentAccelaration();
        void updateTransformMarixes();
        void updateTorques(float force[6]);
};


void multiply(float matrix[3][3], float* src_vec,float* dst_vec);
}
#endif
