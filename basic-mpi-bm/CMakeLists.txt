#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 2.6)
#Name your project here
project(mpi-bm)

#include headers from this folder
#include_directories(${UTIL_SOURCE_DIR}/include)

set(CMAKE_CXX_COMPILER mpic++)
set(CMAKE_CXX_FLAGS -lnuma)
#Sends the -std=c99 flag to the gcc compiler
add_definitions(-std=c++11 -Wall -Wextra -g -O1 -lstdc++ -lnuma)
#This tells CMake to do executable
add_executable(mpi-bm main.cpp dataManager.cpp result.cpp parameters.cpp)
