# Catkin User Guide: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/user_guide.html
# Catkin CMake Standard: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/standards.html
cmake_minimum_required(VERSION 2.8.3)
project(videoread)
find_package(catkin REQUIRED COMPONENTS roscpp image_transport cv_bridge
	resources OpenCV)

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# ensure CMake creates the bin/ directory automatically
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(videoread src/videoread.cpp)

catkin_package(
    DEPENDS roscpp image_transport cv_bridge resources
    CATKIN_DEPENDS # TODO
    INCLUDE_DIRS # TODO include
    LIBRARIES # TODO
)
include_directories(include ${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS}
	${OpenCV_INCLUDE_DIRS})
target_link_libraries(videoread ${catkin_LIBRARIES} resources
	${OpenCV_INCLUDE_DIRS})
