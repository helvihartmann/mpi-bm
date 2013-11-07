#!/bin/bash

#SBATCH -N3 -n3
#SBATCH -o 403_512MB.out
#SBATCH -J 403.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 ~/firstests/403/403 512000000

exit 0