# Setup
cmake_minimum_required(VERSION 3.0)
project(echo)

# Finding libraries
set(WPI_PATH include/wiringPi/wiringPi)
include_directories(${WPI_PATH})
find_library(WPI_LIB wiringPi HINTS ${WPI_PATH} NO_CMAKE_FIND_ROOT_PATH)
if(NOT WPI_LIB)
    message(FATAL_ERROR "wiringPi library not found")
endif()
find_library(WSER_LIB wiringSerial HINTS ${WPI_PATH} NO_CMAKE_FIND_ROOT_PATH)
if(NOT WSER_LIB)
    message(FATAL_ERROR "wiringSerial library not found")
endif()


# Compiling files
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build)
FILE(GLOB SRC_FILES src/*.c)
add_executable(echo ${SRC_FILES})

target_link_libraries(echo ${WPI_LIB})
