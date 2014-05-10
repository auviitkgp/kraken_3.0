#ifndef _AUV_MODEL_H
#define _AUV_MODEL_H
#include <math.h>
#include <cstdlib>
#include <iostream>
namespace ikat_simulator{

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
        void initAuv();
        void updateCurrentPosition();
        void updateCurrentVelocity();
        void updateCurrentAccelaration();
        void updateTransformMarixes();
        void updateTorques(float force[6]);
};


void multiply(float matrix[3][3], float* src_vec,float* dst_vec);

/*
class AuvModelWater6DoF
{
    public:
        AuvModelWater6DoF(float time);
        ~AuvModelWater6DoF();
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
        void updateAuv(float force[6]);
    private:
        float _time;
        /***** matrix thrust to force/torque ****
        static const float L[6][6];
        /***** Gravity matrix ****
        static const float G[6] ;
        /***** Mass matrix ****
        static const float mass[6][6];
        /***** Damping matrix ****
        float D[6][6];
        void initAuv();
        void updateCurrentPosition();
        void updateCurrentVelocity();
        void updateCurrentAccelaration();
        void updateTransformMarixes();
        void updateDampingTorque();
        void updateTorques(float force[6]);
};

const float AuvModelWater6DoF::mass[6][6] = { { 1, 0, 0, 0, 0, 0},
                                               { 0, 1, 0, 0, 0, 0},
                                               { 0, 0, 1, 0, 0, 0},
                                               { 0, 0, 0, 1, 0, 0},
                                               { 0, 0, 0, 0, 1, 0},
                                               { 0, 0, 0, 0, 0, 5}
                                             };
const float AuvModelWater6DoF::G[6] = {0, 0, -.15, 0, 0, 0};
const float AuvModelWater6DoF::L[6][6]={ { 1, 1, 0, 0, 0, 0},
                                          { 0, 0, 1, 1, 0, 0},
                                          { 0, 0, 0, 0, 1, 1},
                                          { 0, 0, 1, -1, 0, 0},
                                          { 0, 0, 0, 0, 1, -1},
                                          { 1, -1, 0, 0, 0, 0}
                                        };

*/
}// end namespace ikat_simulator
#endif
