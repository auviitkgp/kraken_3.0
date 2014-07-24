#!/bin/bash
WS=$ROS_WORKSPACE 
source /opt/ros/hydro/setup.bash
export ROS_WORKSPACE=$WS
export ROS_PACKAGE_PATH=$ROS_WORKSPACE:$ROS_PACKAGE_PATH

roscd resources
make -j4
./bin/getPythonHeader include/resources/topicHeader.h src/resources/topicHeader.py
