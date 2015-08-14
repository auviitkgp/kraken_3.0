/*

	 A topic is grouped taking into account its publisher. So the problem might arise if there is no unique publisher to a topic.
	 In that case, we may use node remapping. Hence the topics can be grouped based in modules of our design and not on packages.

*/
#include "resources/topicHeader.h"

namespace topics
{
//---simulator topics
//all topics here are only used for simulator and in genral not used by other packages
const char* SIMULATOR_THRUSTER4_TO_FORCE="/kraken/simulator/thruster4toforce";
const char* SIMULATOR_THRUSTER6_TO_FORCE="/kraken/simulator/thruster6toforce";
const char* SIMULATOR_MODEL_POSE="/kraken/simulator/pose";
const char* SIMULATOR_MODEL_FORCE_DATA_4_THRUSTERS="/kraken/physics_model/forceData4Thruster";
const char* SIMULATOR_MODEL_FORCE_DATA_6_THRUSTERS="/kraken/physics_model/forceData6Thruster";
const char* SIMULATOR_MODEL_DATA_NAVIGATOR="/kraken/physics_model/dataNavigator";
const char* SIMULATOR_MODEL_IMU_DATA="/kraken/physics_model/imuData";
const char* SIMULATOR_MODEL_TWIST="/kraken/simulator/twist";
const char* SIMULATOR_JOINT_STATE="/kraken/simulator/joint_state";
const char* SIMULATOR_JOINT_STATE_CMD="/kraken/simulator/joint_state_command";
const char* SIMULATOR_TWIST="/kraken/simulator/twist";
const char* SIMULATOR_POSE="/kraken/simulator/pose";
const char* SIMULATOR_DATA_NAVIGATOR="/kraken/simulator/dataNavigator";
const char* SIMULATOR_IMU="/kraken/simulator/imu";
const char* SIMULATOR_DVL="/kraken/simulator/dvl";
const char* SIMULATOR_DEPTH="/kraken/simulator/depth";



//---controller topics

const char* CONTROL_PID_THRUSTER6="/kraken/control/thruster6pid";
const char* CONTROL_PID_THRUSTER4="/kraken/control/thruster4pid";

const char* CONTROL_SETPOINT_ACTION="/kraken/control/setpoint_action";
const char* CONTROL_ADVANCEDCONTROLLER_ACTION="/kraken/control/advancedcontrol_action";
const char* CONTROL_MOVEALONG_SERV="/kraken/control/move_along_serv";
const char* CONTROL_IP_ERROR="/kraken/control/ip_error";
const char* CONTROL_SWITCH_CONTROL="/kraken/control/switch_srv";


const char* CONTROL_SEABOTIX="/kraken/control/seabotixData";

const char* CONTROL_LOADPARAM="/kraken/control/loadparam_srv";



//sensor topics
const char* SENSOR_IMU="/kraken/sensor/imu";
const char* SENSOR_DEPTH="/kraken/sensor/depth";
const char* SENSOR_DVL="/kraken/sensor/dvl";


//hardware-stack
const char* HARDWARE_BATTERY1="/kraken/hardware/bat1";
const char* HARDWARE_BATTERY2="/kraken/hardware/bat2";
const char* HARDWARE_MISSION_SWITCH_SERV="/kraken/hardware/mission_switch_serv";
const char* HARDWARE_KILL_SWITCH="/kraken/hardware/kill_switch_serv";



//navigation
const char* NAV_POSE_ESTIMATED="/kraken/nav/pose_estimated";
const char* PREMAP_LOCATION_SERVICE="/kraken/nav/premap_location";
const char* RESET_POSITION_SERVICE="/kraken/nav/reset_position";


// pose server
const char* POSE_SERVER_STATE="/kraken/estimated/state";
const char* PRESENT_POSE="/kraken/estimated/position";

// absolute roll, pitch and yaw
// The IMU takes Clockwise angle to be positive;
// On this topic, the anticlockwise roll, pitch and yaw are published.
//
const char* ABSOLUTE_RPY="/kraken/imu/corrected/rpy";

// vision stack topics

const char* CAMERA_BOTTOM_RAW_IMAGE="/kraken/bottom_camera";
const char* CAMERA_BOTTOM_COMPRESSED="/kraken/bottom_camera/compressed";
const char* CAMERA_BOTTOM_COMPRESSED_PARAM_DESCRIP="/kraken/bottom_camera/compressed/parameter_descriptions";
const char* CAMERA_BOTTOM_COMPRESSED_PARAM_UPDATES="/kraken/bottom_camera/compressed/parameter_updates";
const char* CAMERA_BOTTOM_COMPRESSED_DEPTH="/kraken/bottom_camera/compressedDepth";
const char* CAMERA_BOTTOM_COMPRESSED_DEPTH_PARAM_DESCRIP="/kraken/bottom_camera/compressedDepth/parameter_descriptions";
const char* CAMERA_BOTTOM_COMPRESSED_DEPTH_PARAM_UPDATES="/kraken/bottom_camera/compressedDepth/parameter_updates";
const char* CAMERA_BOTTOM_THEORA="/kraken/bottom_camera/theora";
const char* CAMERA_BOTTOM_THEORA_PARAM_DESCRIP="/kraken/bottom_camera/theora/parameter_descriptions";
const char* CAMERA_BOTTOM_THEORA_PARAM_UPDATES="/kraken/bottom_camera/theora/parameter_updates";
const char* CAMERA_BOTTOM_INFO="/kraken/bottom_camera_info";

const char* CAMERA_BOTTOM_MARKER_IMAGE="/kraken/bottom_camera/marker_image";
const char* CAMERA_BOTTOM_VGATE_IMAGE="/kraken/bottom_camera/validation_gate_image";

const char* CAMERA_FRONT_RAW_IMAGE="/kraken/front_camera";
const char* CAMERA_FRONT_COMPRESSED="/kraken/front_camera/compressed";
const char* CAMERA_FRONT_COMPRESSED_PARAM_DESCRIP="/kraken/front_camera/compressed/parameter_descriptions";
const char* CAMERA_FRONT_COMPRESSED_PARAM_UPDATES="/kraken/front_camera/compressed/parameter_updates";
const char* CAMERA_FRONT_COMPRESSED_DEPTH="/kraken/front_camera/compressedDepth";
const char* CAMERA_FRONT_COMPRESSED_DEPTH_PARAM_DESCRIP="/kraken/front_camera/compressedDepth/parameter_descriptions";
const char* CAMERA_FRONT_COMPRESSED_DEPTH_PARAM_UPDATES="/kraken/front_camera/compressedDepth/parameter_updates";
const char* CAMERA_FRONT_THEORA="/kraken/front_camera/theora";
const char* CAMERA_FRONT_THEORA_PARAM_DESCRIP="/kraken/front_camera/theora/parameter_descriptions";
const char* CAMERA_FRONT_THEORA_PARAM_UPDATES="/kraken/front_camera/theora/parameter_updates";
const char* CAMERA_FRONT_INFO="/kraken/front_camera_info";

const char* CAMERA_FRONT_BUOY_IMAGE="/kraken/front_camera/buoy_image";

const char* CAMERA_CAM_SWITCH="/kraken/camera/camera_switch";
}
