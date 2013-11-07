#!/bin/bash

#SBATCH -N8 -n8
#SBATCH -o 403_8MB.out
#SBATCH -J 403.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 ~/firstests/403/403 8000000

exit 0