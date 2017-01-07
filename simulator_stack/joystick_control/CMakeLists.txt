# Catkin User Guide: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/user_guide.html
# Catkin CMake Standard: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/standards.html
cmake_minimum_required(VERSION 2.8.3)
project(joystick_control)
# Load catkin and all dependencies required for this package
find_package(catkin REQUIRED COMPONENTS roscpp rospy kraken_msgs joy resources)

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# ensure CMake creates the bin/ directory automatically
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(joystick_control src/joystick_control.cpp)
add_executable(keyboard_control src/keyboard_control.cpp)

catkin_package(
    DEPENDS roscpp rospy kraken_msgs joy resources
    CATKIN_DEPENDS roscpp rospy kraken_msgs joy resources # TODO
		INCLUDE_DIRS # TODO
    LIBRARIES # TODO
)

include_directories(${catkin_INCLUDE_DIRS})

add_dependencies(joystick_control kraken_msgs_gencpp)
add_dependencies(keyboard_control kraken_msgs_gencpp)

target_link_libraries(joystick_control ${catkin_LIBRARIES} resources)
target_link_libraries(keyboard_control ${catkin_LIBRARIES} resources)
