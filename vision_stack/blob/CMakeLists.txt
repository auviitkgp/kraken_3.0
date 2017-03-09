# Catkin User Guide: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/user_guide.html
# Catkin CMake Standard: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/standards.html
cmake_minimum_required(VERSION 2.8.3)
project(blob)
# Load catkin and all dependencies required for this package
# TODO: remove all from COMPONENTS that are not catkin packages.
find_package(catkin REQUIRED COMPONENTS roscpp std_msgs)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# ensure CMake creates the bin/ directory automatically
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(${Boost_INCLUDE_DIRS})

add_library(${PROJECT_NAME} src/blob.cpp src/BlobContour.cpp src/BlobOperators.cpp src/BlobProperties.cpp src/BlobResult.cpp src/ComponentLabeling.cpp src/testblob.cpp)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES})

add_executable(testblob src/testblob.cpp)

target_link_libraries(testblob ${PROJECT_NAME} ${OpenCV_LIBRARIES}
	${catkin_LIBRARIES})

catkin_package(
    DEPENDS roscpp std_msgs
    CATKIN_DEPENDS # TODO
    INCLUDE_DIRS include # TODO include
    LIBRARIES # TODO
)

include_directories(include ${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS})
