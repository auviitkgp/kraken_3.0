# Catkin User Guide: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/user_guide.html
# Catkin CMake Standard: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/standards.html
cmake_minimum_required(VERSION 2.8.3)
project(seabotix)
find_package(catkin REQUIRED COMPONENTS rospy roslib std_msgs kraken_msgs resources dynamic_reconfigure)


#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(cfg ${Boost_INCLUDE_DIRS})
catkin_package(
    DEPENDS rospy roslib std_msgs kraken_msgs resources dynamic_reconfigure
    CATKIN_DEPENDS # TODO
    INCLUDE_DIRS # TODO
    LIBRARIES # TODO
)
