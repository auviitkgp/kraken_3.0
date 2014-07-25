/** All topic names will be named here. And everywhere wherever we use topic names include this header file and 
use the names from here */

#ifndef TOPICS_HEADER_H
#define TOPICS_HEADER_H 

#include <iostream>

namespace topics{
//---simulator topics
const char* SIMULATOR_POSE="/kraken/simulator/pose";
const char* SIMULATOR_THRUSTER4_TO_FORCE="/kraken/simulator/thruster4toforce";
const char* SIMULATOR_THRUSTER6_TO_FORCE="/kraken/simulator/thruster6toforce";
const char* SIMULATOR_MODEL_POSE="/kraken/physics_model/pose";
const char* SIMULATOR_MODEL_FORCE_DATA_4_THRUSTERS="/kraken/physics_model/forceData4Thruster";
const char* SIMULATOR_MODEL_FORCE_DATA_6_THRUSTERS="/kraken/physics_model/forceData6Thruster";
const char* SIMULATOR_MODEL_DATA_NAVIGATOR="/kraken/physics_model/dataNavigator";
const char* SIMULATOR_MODEL_IMU_DATA="/kraken/physics_model/imuData";
const char* SIMULATOR_MODEL_TWIST="/kraken/physics_model/twist";




//---controller topics
const char* CONTROL_POSE_ESTIMATED="/kraken/control/pose_estimated";
const char* CONTROL_PID_THRUSTER6="/kraken/control/thruster6pid";
const char* CONTROL_PID_THRUSTER4="/kraken/control/thruster4pid";

const char* CONTROL_SETPOINT_ACTION="/kraken/control/setpoint_action";
const char* CONTROL_ADVANCEDCONTROLLER_ACTION="/kraken/control/advancedcontrol_action";
const char* CONTROL_MOVEALONG_SERV="/kraken/control/move_along_serv";
const char* CONTROL_IP_ERROR="/kraken/control/ip_error";
const char* CONTROL_SWITCH_CONTROL="/kraken/control/switch_srv";

const char* CONTROL_SEABOTIX="kraken/control/seabotixData";


//sensor topics
const char* SENSOR_IMU="/kraken/sensor/imu_data";
const char* SENSOR_DEPTH="/kraken/sensor/depth_data";
const char* SENSOR_DVL="/kraken/sensor/velocity_data";


//hardware-stack
const char* HARDWARE_BATTERY1="/kraken/hardware/bat1";
const char* HARDWARE_BATTERY2="/kraken/hardware/bat2";
const char* HARDWARE_MISSION_SWITCH_SERV="/kraken/hardware/mission_switch_serv";
const char* HARDWARE_KILL_SWITCH="/kraken/hardware/kill_switch_serv";

// vision stack topics

const char* CAMERA_BOTTOM_RAW_IMAGE='kraken/camera/bottomcam/raw_image';
const char* CAMERA_CAM_SWITCH='kraken/camera/bottomcam/camera_switch';

//navigation


}
#endif
