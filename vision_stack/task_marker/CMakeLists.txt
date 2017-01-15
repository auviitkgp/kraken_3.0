# Catkin User Guide: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/user_guide.html
# Catkin CMake Standard: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/standards.html
cmake_minimum_required(VERSION 2.8.3)
project(task_marker)
# Load catkin and all dependencies required for this package
# TODO: remove all from COMPONENTS that are not catkin packages.
find_package(catkin REQUIRED COMPONENTS roscpp actionlib ip_msgs sensor_msgs image_transport cv_bridge blob resources)

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_executable(server src/marker_server.cpp)
add_executable(client src/marker_client.cpp)

add_dependencies(server kraken_msgs_gencpp ip_msgs_gencpp actionmsg_gencpp)
add_dependencies(client kraken_msgs_gencpp ip_msgs_gencpp actionmsg_gencpp)

catkin_package(
    DEPENDS roscpp actionlib ip_msgs sensor_msgs image_transport cv_bridge blob resources
    CATKIN_DEPENDS # TODO
    INCLUDE_DIRS include # TODO include
    LIBRARIES # TODO
)

include_directories(include ${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS})

target_link_libraries(server ${catkin_LIBRARIES} resources blob)
target_link_libraries(client ${catkin_LIBRARIES} resources)
