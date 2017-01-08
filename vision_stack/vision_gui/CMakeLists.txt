# Catkin User Guide: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/user_guide.html
# Catkin CMake Standard: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/standards.html
cmake_minimum_required(VERSION 2.8.3)
project(vision_gui)
# Load catkin and all dependencies required for this package
# TODO: remove all from COMPONENTS that are not catkin packages.
find_package(catkin REQUIRED COMPONENTS roscpp cv_bridge blob resources)

# include_directories(include ${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS})set(CMAKE_AUTOMOC ON)

##############################################################################
# Rosbuild2
##############################################################################

if(ROSBUILD)

# CATKIN_MIGRATION: removed during catkin migration
#   include(rosbuild.cmake OPTIONAL)
  return()
endif()

##############################################################################
# CMake
##############################################################################


# CATKIN_MIGRATION: removed during catkin migration
# cmake_minimum_required(VERSION 2.4.6)

##############################################################################
# Qt Environment
##############################################################################

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ )
FIND_PACKAGE(Qt4 REQUIRED)
#find_package(Qwt REQUIRED)
find_package( OpenCV REQUIRED )
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(-DQT_NO_KEYWORDS)
include_directories(include/${PROJECT_NAME}/ ${CMAKE_CURRENT_BINARY_DIR})

##############################################################################
# Ros Initialisation
##############################################################################


# CATKIN_MIGRATION: removed during catkin migration
# include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# CATKIN_MIGRATION: removed during catkin migration
# rosbuild_init()

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE Debug)

##############################################################################
# Sections
##############################################################################

file(GLOB QT_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
file(GLOB QT_RESOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} resources/*.qrc)
file(GLOB_RECURSE QT_MOC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS include/${PROJECT_NAME}/*.hpp)

QT4_ADD_RESOURCES(QT_RESOURCES_CPP ${QT_RESOURCES})
QT4_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
QT4_WRAP_CPP(QT_MOC_HPP ${QT_MOC})

##############################################################################
# Sources
##############################################################################

file(GLOB_RECURSE QT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS src/*.cpp)

##############################################################################
# Binaries
##############################################################################

add_executable(App ${QT_SOURCES} ${QT_RESOURCES_CPP} ${QT_FORMS_HPP} ${QT_MOC_HPP})
target_link_libraries(App ${QT_LIBRARIES} ${OpenCV_LIBS})
## Generate added messages and services with any dependencies listed here
#generate_messages(
#    #TODO DEPENDENCIES geometry_msgs std_msgs
#)

# catkin_package parameters: http://ros.org/doc/groovy/api/catkin/html/dev_guide/generated_cmake_api.html#catkin-package
# TODO: fill in what other packages will need to use this package
catkin_package(
    DEPENDS roscpp cv_bridge blob resources
    CATKIN_DEPENDS # TODO
    INCLUDE_DIRS include # TODO include
    LIBRARIES # TODO
)
include_directories(include ${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS})
