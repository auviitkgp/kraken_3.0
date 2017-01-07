# Catkin User Guide: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/user_guide.html
# Catkin CMake Standard: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/standards.html
cmake_minimum_required(VERSION 2.8.3)
project(SerialPort)
find_package(catkin REQUIRED COMPONENTS roscpp resources)

include_directories(include ${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS})
#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# ensure CMake creates the bin/ directory automatically
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_library(${PROJECT_NAME} src/SerialPort.cpp)

target_link_libraries(${PROJECT_NAME} log4cxx)

add_executable(serialport_test src/test.cpp)

target_link_libraries(serialport_test ${PROJECT_NAME})
target_link_libraries(serialport_test ${catkin_LIBRARIES} resources)

catkin_package(
    DEPENDS roscpp resources
    CATKIN_DEPENDS roscpp resources # TODO
    INCLUDE_DIRS include # TODO include
    LIBRARIES # TODO
)
