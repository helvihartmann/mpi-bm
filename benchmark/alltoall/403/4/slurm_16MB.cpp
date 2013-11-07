#!/bin/bash

#SBATCH -N4 -n4
#SBATCH -o 403_16MB.out
#SBATCH -J 403.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 ~/firstests/403/403 16000000

exit 0