cmake_minimum_required(VERSION 3.16.2)
project(flir_ros_cam VERSION 1.0.0)
set(flir_ros_cam ${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_BUILD_TYPE Debug)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")

else()
  set(OPTIMIZATION_FLAGS $ENV{CPP_OPTIMIZATIONS})
  set(
    CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -DNDEBUG -O3 -Wno-deprecated-declarations -flto"
  )
endif()

message(STATUS "DEBUG_TIME_LOGGING is ${DEBUG_TIME_LOGGING}")
message(STATUS "DEBUG_SINGLE_IMAGE is ${DEBUG_SINGLE_IMAGE}")

set(CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cfg)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.h.in
  ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h
)

# ============ Dependencies ============

link_directories(/usr/local/lib)
if(POLICY CMP0146)
  cmake_policy(SET CMP0146 OLD)
endif()
set(OPENCV_VERSION $ENV{OPENCV_VERSION})
message(STATUS "OpenCV version: ${OPENCV_VERSION}")
find_package(OpenCV ${OPENCV_VERSION} REQUIRED)

find_package(Eigen3 REQUIRED NO_MODULE)
find_package(cpp_utils REQUIRED)
find_package(flirmulticamera REQUIRED)
find_package(fmt REQUIRED)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  ros_node_interface
  sensor_msgs
  keiko_msgs
)

catkin_package(
  INCLUDE_DIRS src
  CATKIN_DEPENDS roscpp ros_node_interface keiko_msgs
)

set(
  IncludeDirsList
  ${catkin_LIBRARIES} 
  ${SRC_DIR}
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)
include_directories(
  ${catkin_LIBRARIES} 
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(
  LibrariesList 
  ${OpenCV_LIBS}
  ${CMAKE_THREAD_LIBS_INIT}
  ${catkin_LIBRARIES}
  cpp_utils flirmulticamera
)

set(
  DependenciesList ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS}
)

# ============ EXECUTABLES ============
# INTERFACE
add_executable(interface src/nodes/interface.cpp src/flir_ros_cam.cpp)
add_dependencies(interface ${DependenciesList})
target_include_directories(interface PUBLIC ${IncludeDirsList})
target_link_libraries(interface PUBLIC ${LibrariesList})
set_target_properties(interface PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
