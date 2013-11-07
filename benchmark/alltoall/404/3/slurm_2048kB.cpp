#!/bin/bash

#SBATCH -N3 -n3
#SBATCH -o 404_2048kB.out
#SBATCH -J 404.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 ~/firstests/404/404 2048000

exit 0