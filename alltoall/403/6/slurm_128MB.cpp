#!/bin/bash

#SBATCH -N6 -n6
#SBATCH -o 403_128MB.out
#SBATCH -J 403.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 ~/firstests/403/403 128000000

exit 0