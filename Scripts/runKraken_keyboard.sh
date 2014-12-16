#!/bin /bash
# ! /bin/zsh
# WS=$ROS_WORKSPACE 
# source /opt/ros/hydro/setup.zsh
# export ROS_WORKSPACE=$WS
# export ROS_PACKAGE_PATH=$ROS_WORKSPACE:$ROS_PACKAGE_PATH

# Run kraken_visualisation module
roscd
cd kraken_3.0/Scripts/
./kraken_visualization_launcher &

#Run simulator physics model
rosrun simulator_physics_model start_auv_model /kraken/physics_model/pose:=/kraken/simulator/pose &

#Run keyboard controller module
rosrun joystick_control keyboard_control &

roscd
cd kraken_3.0/simulator_stack/joystick_control/src/
python main.py

