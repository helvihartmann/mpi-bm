#!/bin/bash

#SBATCH -N4 -n4
#SBATCH -o 403_256MB.out
#SBATCH -J 403.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 ~/firstests/403/403 256000000

exit 0