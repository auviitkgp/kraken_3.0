# Catkin User Guide: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/user_guide.html
# Catkin CMake Standard: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/standards.html
cmake_minimum_required(VERSION 2.8.3)
project(bottomcam)
find_package(catkin REQUIRED COMPONENTS roscpp std_msgs image_transport cv_bridge resources rostest)

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# ensure CMake creates the bin/ directory automatically
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(start_camera src/main.cpp src/bottomcam.cpp)

target_link_libraries(start_camera ${catkin_LIBRARIES} resources)

catkin_package(
    DEPENDS roscpp std_msgs image_transport cv_bridge resources rostest
    CATKIN_DEPENDS # TODO
    INCLUDE_DIRS # TODO include
    LIBRARIES # TODO

)

include_directories(include ${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS})
catkin_add_gtest(utest test/test_cam_opens.cpp src/bottomcam.cpp)
