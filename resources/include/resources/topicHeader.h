/** All topic names will be named here. And everywhere wherever we use topic names include this header file and 
use the names from here */

#ifndef TOPICS_HEADER_H
#define TOPICS_HEADER_H 

#include <iostream>

namespace topics{
//---simulator topics
extern const char* SIMULATOR_POSE;
extern const char* SIMULATOR_THRUSTER4_TO_FORCE;
extern const char* SIMULATOR_THRUSTER6_TO_FORCE;
extern const char* SIMULATOR_MODEL_POSE;
extern const char* SIMULATOR_MODEL_FORCE_DATA_4_THRUSTERS;
extern const char* SIMULATOR_MODEL_FORCE_DATA_6_THRUSTERS;
extern const char* SIMULATOR_MODEL_DATA_NAVIGATOR;
extern const char* SIMULATOR_MODEL_IMU_DATA;
extern const char* SIMULATOR_MODEL_TWIST;




//---controller topics
extern const char* CONTROL_POSE_ESTIMATED;
extern const char* CONTROL_PID_THRUSTER6;
extern const char* CONTROL_PID_THRUSTER4;

extern const char* CONTROL_SETPOINT_ACTION;
extern const char* CONTROL_ADVANCEDCONTROLLER_ACTION;
extern const char* CONTROL_MOVEALONG_SERV;
extern const char* CONTROL_IP_ERROR;
extern const char* CONTROL_SWITCH_CONTROL;



extern const char* CONTROL_SEABOTIX;



//sensor topics
extern const char* SENSOR_IMU;
extern const char* SENSOR_DEPTH;
extern const char* SENSOR_DVL;


//hardware-stack
extern const char* HARDWARE_BATTERY1;
extern const char* HARDWARE_BATTERY2;
extern const char* HARDWARE_MISSION_SWITCH_SERV;
extern const char* HARDWARE_KILL_SWITCH;

// vision stack topics


extern const char* CAMERA_BOTTOM_RAW_IMAGE;
extern const char* CAMERA_CAM_SWITCH;
extern const char* CAMERA_FRONT_RAW_IMAGE;

//navigation


}
#endif
