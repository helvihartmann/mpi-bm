#!/bin/bash

#SBATCH -N5 -n5
#SBATCH -o 404_512kB.out
#SBATCH -J 404.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 ~/firstests/404/404 512000

exit 0