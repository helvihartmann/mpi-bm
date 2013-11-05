#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 404_131072kB.out
#SBATCH -J 404.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 ~/firstests/404/404 131072000

exit 0