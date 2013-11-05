#!/bin/bash

#SBATCH -N6 -n6
#SBATCH -o 403_4MB.out
#SBATCH -J 403.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 ~/firstests/403/403 4000000

exit 0