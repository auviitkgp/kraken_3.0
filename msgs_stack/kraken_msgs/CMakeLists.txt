cmake_minimum_required(VERSION 2.8.3)
project(kraken_msgs)
find_package(catkin REQUIRED COMPONENTS 
	actionlib 
	actionlib_msgs 
	geometry_msgs
	nodelet
	std_msgs
	roscpp 
	rospy 
	message_generation)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_action_files(
	DIRECTORY action
)

add_message_files(
	DIRECTORY msg
)

add_service_files(
	DIRECTORY srv
)

include_directories(${Boost_INCLUDE_DIRS} ${catkin_INCLUDE_DIRS})

generate_messages(
	DEPENDENCIES actionlib_msgs geometry_msgs
)

catkin_package(
    #DEPENDS actionlib actionlib_msgs std_msgs geometry_msgs sensor_msgs roscpp rospy
    #CATKIN_DEPENDS message_runtime # TODO
    #INCLUDE_DIRS # TODO include
    #LIBRARIES # TODO
)
