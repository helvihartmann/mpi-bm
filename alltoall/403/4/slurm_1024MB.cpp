#!/bin/bash

#SBATCH -N4 -n4
#SBATCH -o 403_1024MB.out
#SBATCH -J 403.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 ~/firstests/403/403 1024000000

exit 0