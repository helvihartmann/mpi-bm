#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 2.6)
#Name your project here
project(multinodes)

#include headers from this folder
#include_directories(${UTIL_SOURCE_DIR}/include)

#set(CMAKE_CXX_COMPILER mpic++)
set(MPIPATH "/opt/openmpi185")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11 -Wall -Wextra -g -O1 -Wl,-rpath -Wl,${MPIPATH}/lib64 -Wl,--enable-new-dtags")

include_directories(${MPIPATH}/include)
#This tells CMake to do executable
add_executable(multinodes multinodes.cpp buffer.cpp parameters.cpp results.cpp measurement.cpp addfunctions.cpp)
target_link_libraries(multinodes ${MPIPATH}/lib64/libmpi.so ${MPIPATH}/lib64/libmpi_cxx.so numa)
