# Catkin User Guide: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/user_guide.html
# Catkin CMake Standard: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/standards.html
cmake_minimum_required(VERSION 2.8.3)
project(thruster_converter)
# Load catkin and all dependencies required for this package
# TODO: remove all from COMPONENTS that are not catkin packages.
find_package(catkin REQUIRED COMPONENTS roscpp std_msgs kraken_msgs SerialPort
	resources message_generation)

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# ensure CMake creates the bin/ directory automatically
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(${Boost_INCLUDE_DIRS} ${catkin_INCLUDE_DIRS})

add_executable(thrust_convert src/thruster.cpp)

add_dependencies(thrust_convert kraken_msgs_gencpp)

target_link_libraries(thrust_convert resources ${catkin_LIBRARIES})

catkin_package(
    DEPENDS roscpp std_msgs kraken_msgs SerialPort resources
    CATKIN_DEPENDS # TODO
    INCLUDE_DIRS # TODO include
    LIBRARIES # TODO
)
