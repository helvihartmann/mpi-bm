#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 403_1MB.out
#SBATCH -J 403.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 ~/firstests/403/403 1000000

exit 0