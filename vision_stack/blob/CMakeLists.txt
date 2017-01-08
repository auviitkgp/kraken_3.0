# Catkin User Guide: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/user_guide.html
# Catkin CMake Standard: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/standards.html
cmake_minimum_required(VERSION 2.8.3)
project(blob)
# Load catkin and all dependencies required for this package
# TODO: remove all from COMPONENTS that are not catkin packages.
find_package(catkin REQUIRED COMPONENTS roscpp std_msgs)


# CATKIN_MIGRATION: removed during catkin migration
# cmake_minimum_required(VERSION 2.4.6)

# CATKIN_MIGRATION: removed during catkin migration
# include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE RelWithDebInfo)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

# CATKIN_MIGRATION: removed during catkin migration
# rosbuild_init()

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#uncomment if you have defined messages
#add_message_files(
  #FILES
  # TODO: List your msg files here
#)
#uncomment if you have defined services
#add_service_files(
  #3FILES
  # TODO: List your msg files here
#)

#common commands for building c++ executables and libraries
#add_library(${PROJECT_NAME} src/example.cpp)
#target_link_libraries(${PROJECT_NAME} another_library)
#
# CATKIN_MIGRATION: removed during catkin migration
# rosbuild_add_boost_directories()
#find_package(Boost REQUIRED COMPONENTS thread)
include_directories(${Boost_INCLUDE_DIRS})
#target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
#add_executable(example examples/example.cpp)
#target_link_libraries(example ${PROJECT_NAME})
add_library(${PROJECT_NAME} src/blob.cpp src/BlobContour.cpp src/BlobOperators.cpp src/BlobProperties.cpp src/BlobResult.cpp src/ComponentLabeling.cpp src/testblob.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES})

#add_library(blobcontour src/BlobContour.cpp)
#add_library(bloboperators src/BlobOperators.cpp)
#add_library(blobprop src/BlobProperties.cpp)
#add_library(blobresult src/BlobResult.cpp)
#add_library(component src/ComponentLabeling.cpp)
add_executable(testblob src/testblob.cpp)
target_link_libraries(testblob ${PROJECT_NAME})

target_link_libraries(testblob ${OpenCV_LIBRARIES})

#target_link_libraries(testblob blobcontour)
#target_link_libraries(testblob bloboperators)
#target_link_libraries(testblob blobprop)
#target_link_libraries(testblob blobresult)
#target_link_libraries(testblob component)
## Generate added messages and services with any dependencies listed here
#generate_messages(
    #TODO DEPENDENCIES geometry_msgs std_msgs
#)

# catkin_package parameters: http://ros.org/doc/groovy/api/catkin/html/dev_guide/generated_cmake_api.html#catkin-package
# TODO: fill in what other packages will need to use this package
catkin_package(
    DEPENDS roscpp std_msgs
    CATKIN_DEPENDS # TODO
    INCLUDE_DIRS include # TODO include
    LIBRARIES # TODO
)
include_directories(include ${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS})

target_link_libraries(testblob ${catkin_LIBRARIES})