#!/bin/bash

#SBATCH -N5 -n5
#SBATCH -o 403_128MB.out
#SBATCH -J 403.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 ~/firstests/403/403 128000000

exit 0