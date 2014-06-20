#!/bin/bash
WS=$ROS_WORKSPACE 
source /opt/ros/hydro/setup.bash
export ROS_WORKSPACE=$WS
export ROS_PACKAGE_PATH=$ROS_WORKSPACE:$ROS_PACKAGE_PATH


#Run kraken_visualisation module
roscd
cd kraken_3.0/Scripts/
./kraken_visualization_launcher &

#Run simulator physics model
rosrun simulator_physics_model start_auv_model &

#Pass parameter for the joystick setup
#Use path to joystick here
rosparam set joy_node/dev "/dev/input/js1"

#Start joystick publisher module
rosrun joy joy_node &

#Run joystick controller module
rosrun joystick_control joystick_control
