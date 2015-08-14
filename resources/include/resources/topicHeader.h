/** All topic names will be named here. And everywhere wherever we use topic names include this header file and
use the names from here */

#ifndef TOPICS_HEADER_H
#define TOPICS_HEADER_H

#include <iostream>

namespace topics
{
//---simulator topics

extern const char* SIMULATOR_THRUSTER4_TO_FORCE;
extern const char* SIMULATOR_THRUSTER6_TO_FORCE;
extern const char* SIMULATOR_MODEL_POSE;
extern const char* SIMULATOR_MODEL_FORCE_DATA_4_THRUSTERS;
extern const char* SIMULATOR_MODEL_FORCE_DATA_6_THRUSTERS;
extern const char* SIMULATOR_MODEL_DATA_NAVIGATOR;
extern const char* SIMULATOR_MODEL_IMU_DATA;
extern const char* SIMULATOR_MODEL_TWIST;
extern const char* SIMULATOR_JOINT_STATE;
extern const char* SIMULATOR_JOINT_STATE_CMD;
extern const char* SIMULATOR_TWIST;
extern const char* SIMULATOR_POSE;
extern const char* SIMULATOR_DATA_NAVIGATOR;
extern const char* SIMULATOR_IMU;
extern const char* SIMULATOR_DVL;
extern const char* SIMULATOR_DEPTH;




//---controller topics

extern const char* CONTROL_PID_THRUSTER6;
extern const char* CONTROL_PID_THRUSTER4;

extern const char* CONTROL_SETPOINT_ACTION;
extern const char* CONTROL_ADVANCEDCONTROLLER_ACTION;
extern const char* CONTROL_MOVEALONG_SERV;
extern const char* CONTROL_IP_ERROR;
extern const char* CONTROL_SWITCH_CONTROL;



extern const char* CONTROL_SEABOTIX;
extern const char* CONTROL_LOADPARAM;


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

/*/kraken/bottom_camera
/kraken/bottom_camera/compressed
/kraken/bottom_camera/compressed/parameter_descriptions
/kraken/bottom_camera/compressed/parameter_updates
/kraken/bottom_camera/compressedDepth
/kraken/bottom_camera/compressedDepth/parameter_descriptions
/kraken/bottom_camera/compressedDepth/parameter_updates
/kraken/bottom_camera/theora
/kraken/bottom_camera/theora/parameter_descriptions
/kraken/bottom_camera/theora/parameter_updates
/kraken/bottom_camera_info
*/
extern const char* CAMERA_BOTTOM_RAW_IMAGE;
extern const char* CAMERA_BOTTOM_COMPRESSED;
extern const char* CAMERA_BOTTOM_COMPRESSED_PARAM_DESCRIP;
extern const char* CAMERA_BOTTOM_COMPRESSED_PARAM_UPDATES;
extern const char* CAMERA_BOTTOM_COMPRESSED_DEPTH;
extern const char* CAMERA_BOTTOM_COMPRESSED_DEPTH_PARAM_DESCRIP;
extern const char* CAMERA_BOTTOM_COMPRESSED_DEPTH_PARAM_UPDATES;
extern const char* CAMERA_BOTTOM_THEORA;
extern const char* CAMERA_BOTTOM_THEORA_PARAM_DESCRIP;
extern const char* CAMERA_BOTTOM_THEORA_PARAM_UPDATES;
extern const char* CAMERA_BOTTOM_INFO;

extern const char* CAMERA_BOTTOM_MARKER_IMAGE;
extern const char* CAMERA_BOTTOM_VGATE_IMAGE;

/*
/kraken/front_camera
/kraken/front_camera/compressed
/kraken/front_camera/compressed/parameter_descriptions
/kraken/front_camera/compressed/parameter_updates
/kraken/front_camera/compressedDepth
/kraken/front_camera/compressedDepth/parameter_descriptions
/kraken/front_camera/compressedDepth/parameter_updates
/kraken/front_camera/theora
/kraken/front_camera/theora/parameter_descriptions
/kraken/front_camera/theora/parameter_updates
/kraken/front_camera_info
*/
extern const char* CAMERA_FRONT_RAW_IMAGE;
extern const char* CAMERA_FRONT_COMPRESSED;
extern const char* CAMERA_FRONT_COMPRESSED_PARAM_DESCRIP;
extern const char* CAMERA_FRONT_COMPRESSED_PARAM_UPDATES;
extern const char* CAMERA_FRONT_COMPRESSED_DEPTH;
extern const char* CAMERA_FRONT_COMPRESSED_DEPTH_PARAM_DESCRIP;
extern const char* CAMERA_FRONT_COMPRESSED_DEPTH_PARAM_UPDATES;
extern const char* CAMERA_FRONT_THEORA;
extern const char* CAMERA_FRONT_THEORA_PARAM_DESCRIP;
extern const char* CAMERA_FRONT_THEORA_PARAM_UPDATES;
extern const char* CAMERA_FRONT_INFO;

extern const char* CAMERA_FRONT_BUOY_IMAGE;

extern const char* CAMERA_CAM_SWITCH;

//navigation
extern const char* NAV_POSE_ESTIMATED;
extern const char* PREMAP_LOCATION_SERVICE;
extern const char* RESET_POSITION_SERVICE;

// pose server
extern const char* POSE_SERVER_STATE;
extern const char* PRESENT_POSE;

// absolute RPY
// Check topicHeader.cpp for more details

extern const char* ABSOLUTE_RPY;
}
#endif
