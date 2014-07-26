WS=$ROS_WORKSPACE 
source /opt/ros/hydro/setup.bash
export ROS_WORKSPACE=$WS
export ROS_PACKAGE_PATH=$ROS_WORKSPACE:$ROS_PACKAGE_PATH


rosmake sensor_stack mission_planner_stack control_system_stack msgs_stack gui_stack hardware_stack vision_stack --threads 4
