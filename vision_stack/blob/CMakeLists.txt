cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE RelWithDebInfo)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})
rosbuild_init()

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#uncomment if you have defined messages
#rosbuild_genmsg()
#uncomment if you have defined services
#rosbuild_gensrv()

#common commands for building c++ executables and libraries
#rosbuild_add_library(${PROJECT_NAME} src/example.cpp)
#target_link_libraries(${PROJECT_NAME} another_library)
#rosbuild_add_boost_directories()
#rosbuild_link_boost(${PROJECT_NAME} thread)
#rosbuild_add_executable(example examples/example.cpp)
#target_link_libraries(example ${PROJECT_NAME})
rosbuild_add_library(${PROJECT_NAME} src/blob.cpp src/BlobContour.cpp src/BlobOperators.cpp src/BlobProperties.cpp src/BlobResult.cpp src/ComponentLabeling.cpp src/testblob.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES})

#rosbuild_add_library(blobcontour src/BlobContour.cpp)
#rosbuild_add_library(bloboperators src/BlobOperators.cpp)
#rosbuild_add_library(blobprop src/BlobProperties.cpp)
#rosbuild_add_library(blobresult src/BlobResult.cpp)
#rosbuild_add_library(component src/ComponentLabeling.cpp)
rosbuild_add_executable(testblob src/testblob.cpp)
target_link_libraries(testblob ${PROJECT_NAME})

target_link_libraries(testblob ${OpenCV_LIBRARIES})

#target_link_libraries(testblob blobcontour)
#target_link_libraries(testblob bloboperators)
#target_link_libraries(testblob blobprop)
#target_link_libraries(testblob blobresult)
#target_link_libraries(testblob component)
