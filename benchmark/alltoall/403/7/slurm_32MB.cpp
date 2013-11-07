#!/bin/bash

#SBATCH -N7 -n7
#SBATCH -o 403_32MB.out
#SBATCH -J 403.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 ~/firstests/403/403 32000000

exit 0