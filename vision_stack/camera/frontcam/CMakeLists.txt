# Catkin User Guide: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/user_guide.html
# Catkin CMake Standard: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/standards.html
cmake_minimum_required(VERSION 2.8.3)
project(frontcam)
find_package(catkin REQUIRED COMPONENTS roscpp cv_bridge image_transport std_msgs resources)

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# ensure CMake creates the bin/ directory automatically
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(frontcam src/frontcam.cpp)

target_link_libraries(frontcam ${catkin_LIBRARIES} resources)

add_dependencies(frontcam kraken_msgs_gencpp action_gencpp)
catkin_package(
    DEPENDS #roscpp cv_bridge image_transport std_msgs resources
    CATKIN_DEPENDS roscpp cv_bridge image_transport std_msgs resources
    INCLUDE_DIRS # TODO include
    LIBRARIES # TODO
)
include_directories(include ${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS})
