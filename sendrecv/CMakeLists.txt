#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 2.6)
#Name your project here
project(pingpong)

#include headers from this folder
#include_directories(${UTIL_SOURCE_DIR}/include)

set(CMAKE_CXX_COMPILER mpic++)
#Sends the -std=c99 flag to the gcc compiler
add_definitions(-std=c++11 -Wall -Wextra -g -O3)
#This tells CMake to do executable
add_executable(pingpong src/pingpong.cpp src/buffer.cpp src/parameters.cpp src/results.cpp)

