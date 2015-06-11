#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 2.6)
#Name your project here
project(mpi-ft)

#include headers from this folder
#include_directories(${UTIL_SOURCE_DIR}/include)

#set(CMAKE_CXX_COMPILER mpic++)
set(MPIPATH "/opt/openmpi183")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11 -Wall -Wextra -g -O1 -Wl,-rpath -Wl,${MPIPATH}/lib64 -Wl,--enable-new-dtags")

include_directories(${MPIPATH}/include)
#This tells CMake to do executable
add_executable(mpi-ft src/main.cpp src/addfunctions.cpp)
target_link_libraries(mpi-ft ${MPIPATH}/lib64/libmpi.so ${MPIPATH}/lib64/libmpi_cxx.so numa)
