#!/bin/bash

#SBATCH -N8 -n8
#SBATCH -o 404_2kB.out
#SBATCH -J 404.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 ~/firstests/404/404 2000

exit 0