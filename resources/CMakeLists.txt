# Catkin User Guide: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/user_guide.html
# Catkin CMake Standard: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/standards.html
cmake_minimum_required(VERSION 2.8.3)
project(resources)
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs)

# set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# ensure CMake creates the bin/ directory automatically
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

#common commands for building c++ executables and libraries
add_library(${PROJECT_NAME} src/topicHeader.cpp src/tools.cpp)

include_directories(${Boost_INCLUDE_DIRS} include)

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

add_executable(getPythonHeader src/getPythonHeader.cpp)

catkin_package(
    DEPENDS roscpp rospy std_msgs
    CATKIN_DEPENDS # TODO
    INCLUDE_DIRS include # TODO include
    LIBRARIES # TODO
)
