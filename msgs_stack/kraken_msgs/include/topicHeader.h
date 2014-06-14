/** All topic names will be named here. And everywhere wherever we use topic names include this header file and 
use the names from here */
#include <iostream>
using std::string;

//---simulator topics
const string SIMULATOR_POSE="/kraken/simulator/pose";
const string SIMULATOR_THRUSTER4_TO_FORCE="/kraken/simulator/thruster4toforce";
const string SIMULATOR_THRUSTER6_TO_FORCE="/kraken/simulator/thruster6toforce";


//---controller topics
const string CONTROL_POSE_ESTIMATED="/kraken/control/pose_estimated";
const string CONTROL_PID_THRUSTER6="/kraken/control/thruster6pid";
const string CONTROL_PID_THRUSTER4="/kraken/control/thruster4pid";

const string CONTROL_SETPOINT_ACTION="/kraken/control/setpoint_action";
const string CONTROL_ADVANCEDCONTROLLER_ACTION="/kraken/control/advancedcontrol_action";
const string CONTROL_MOVEALONG_SERV="/kraken/control/move_along_serv";
const string CONTROL_IP_ERROR="/kraken/control/ip_error";
const string CONTROL_SWITCH_CONTROL="/kraken/control/switch_srv";



//sensor topics
const string SENSOR_IMU="/kraken/sensor/imu_data";
const string SENSOR_DEPTH="/kraken/sensor/depth_data";
const string SENSOR_DVL="/kraken/sensor/velocity_data";
const string SENSOR_DVL="/kraken/sensor/velocity_data";

//hardware-stack
const string HARDWARE_BATTERY1="/kraken/hardware/bat1";
const string HARDWARE_BATTERY2="/kraken/hardware/bat2";
const string HARDWARE_MISSION_SWITCH_SERV="/kraken/hardware/mission_switch_serv";
const string HARDWARE_KILL_SWITCH="/kraken/hardware/kill_switch_serv";



//navigation



