#!/bin/bash

#SBATCH -N8 -n8
#SBATCH -o 403_2048MB.out
#SBATCH -J 403.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 ~/firstests/403/403 2048000000

exit 0