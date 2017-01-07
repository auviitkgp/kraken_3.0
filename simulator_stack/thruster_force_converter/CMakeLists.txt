# Catkin User Guide: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/user_guide.html
# Catkin CMake Standard: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/standards.html
cmake_minimum_required(VERSION 2.8.3)
project(thruster_force_converter)
# Load catkin and all dependencies required for this package
find_package(catkin REQUIRED COMPONENTS roscpp kraken_msgs resources)

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# ensure CMake creates the bin/ directory automatically
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(${Boost_INCLUDE_DIRS} include ${catkin_INCLUDE_DIRS})

add_executable(converter src/main.cpp src/ForceConverter.cpp)
catkin_package(
    DEPENDS roscpp kraken_msgs resources
    CATKIN_DEPENDS # TODO
		INCLUDE_DIRS # TODO
    LIBRARIES # TODO
)

add_dependencies(converter kraken_msgs_gencpp)

target_link_libraries(converter ${catkin_LIBRARIES} resources)
