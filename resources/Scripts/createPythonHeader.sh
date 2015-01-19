#!/bin/bash
WS=$ROS_WORKSPACE 
source /opt/ros/indigo/setup.bash
export ROS_WORKSPACE=$WS
export ROS_PACKAGE_PATH=$ROS_WORKSPACE:$ROS_PACKAGE_PATH

roscd resources

file=./bin/getPythonHeader

if [[ -f $file ]]; then
	echo "file found"
else
	echo "file not found"
	make -j4
fi

 file src/topicHeader.cpp src/resources/topicHeader.py
