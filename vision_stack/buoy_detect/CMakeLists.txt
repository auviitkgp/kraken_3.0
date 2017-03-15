cmake_minimum_required(VERSION 2.8)

project( buoy_detect )
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)
rosbuild_init()
rosbuild_find_ros_package(kraken_msgs)
include_directories(${kraken_msgs_PACKAGE_PATH}/msg_gen/cpp/include/)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

rosbuild_genmsg()

find_package( Threads )
find_package( OpenCV REQUIRED )
set(CMAKE_BUILD_TYPE Debug)
#subdirs(vw_detect region_growing)
#rosbuild_add_executable( test_main  src/main.cpp vw_detect/src/vw_detect.cpp )
rosbuild_add_executable( integrator src/integration.cpp vw_detect/src/vw_detect.cpp
         region_growing/src/grow.cpp region_growing/src/CIEDE2000.cpp region_growing/src/Color.cpp)
include_directories(vw_detect/src)
include_directories(region_growing/src)
#target_link_libraries( test_main ${OpenCV_LIBS} )
#target_link_libraries( test_main ${CMAKE_THREAD_LIBS_INIT} )
target_link_libraries( integrator ${OpenCV_LIBS} )
target_link_libraries( integrator ${CMAKE_THREAD_LIBS_INIT} )
add_definitions(-std=c++14)
