# Catkin User Guide: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/user_guide.html
# Catkin CMake Standard: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/standards.html
cmake_minimum_required(VERSION 2.8.3)
project(task_buoy)

find_package(catkin REQUIRED COMPONENTS roscpp 
	actionlib 
	actionmsg 
	kraken_msgs 
	blob 
	image_transport 
	cv_bridge 
	resources)

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# ensure CMake creates the bin/ directory automatically
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(server_shape src/buoy_server_shape.cpp)
add_executable(server_nn src/buoy_server_nn.cpp)
add_executable(server_dt src/buoy_server_dt.cpp)
add_executable(buoy_client src/buoy_client.cpp)
add_executable(new_client src/task_buoy_new.cpp)

add_dependencies(server_shape kraken_msgs_gencpp actionmsg_gencpp ip_msgs_gencpp)
add_dependencies(server_nn kraken_msgs_gencpp actionmsg_gencpp ip_msgs_gencpp)
add_dependencies(server_dt kraken_msgs_gencpp actionmsg_gencpp ip_msgs_gencpp)
add_dependencies(buoy_client kraken_msgs_gencpp actionmsg_gencpp ip_msgs_gencpp)
add_dependencies(new_client kraken_msgs_gencpp actionmsg_gencpp ip_msgs_gencpp)

catkin_package(
    DEPENDS roscpp actionlib actionmsg kraken_msgs blob image_transport cv_bridge resources
    CATKIN_DEPENDS # TODO
    INCLUDE_DIRS include  # TODO include
    LIBRARIES # TODO
)

include_directories(include ${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS})

target_link_libraries(server_shape ${catkin_LIBRARIES} resources blob)
target_link_libraries(server_nn ${catkin_LIBRARIES} resources blob)
target_link_libraries(server_dt ${catkin_LIBRARIES} resources blob)
target_link_libraries(buoy_client ${catkin_LIBRARIES} resources blob)
target_link_libraries(new_client ${catkin_LIBRARIES} resources blob)
