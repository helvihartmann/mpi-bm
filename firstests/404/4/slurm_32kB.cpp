#!/bin/bash

#SBATCH -N4 -n4
#SBATCH -o 404_32kB.out
#SBATCH -J 404.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 ~/firstests/404/404 32000

exit 0