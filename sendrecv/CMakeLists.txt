#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 2.6)
#Name your project here
project(sendrecv)

#include headers from this folder
#include_directories(${UTIL_SOURCE_DIR}/include)

set(CMAKE_CXX_COMPILER mpic++)
#Sends the -std=c99 flag to the gcc compiler
add_definitions(-std=c++11 -Wall -Wextra)
#This tells CMake to do executable
add_executable(sendrecv sendrecv.cpp bufferoperations.cpp totaldatasendcalc.cpp bandwidthcalc.cpp print.cpp classmpi3.cpp)

