# Catkin User Guide: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/user_guide.html
# Catkin CMake Standard: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/standards.html
cmake_minimum_required(VERSION 2.8.3)
project(actionmsg)
find_package(catkin REQUIRED COMPONENTS actionlib actionlib_msgs std_msgs)

find_package(catkin REQUIRED COMPONENTS actionlib std_msgs message_generation) 

add_action_files(DIRECTORY action)
generate_messages(DEPENDENCIES std_msgs actionlib_msgs)

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# ensure CMake creates the bin/ directory automatically
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

catkin_package(
    DEPENDS actionlib actionlib_msgs std_msgs
    CATKIN_DEPENDS # TODO
    INCLUDE_DIRS # TODO include
    LIBRARIES # TODO
)
