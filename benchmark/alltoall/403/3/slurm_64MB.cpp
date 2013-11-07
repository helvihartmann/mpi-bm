#!/bin/bash

#SBATCH -N3 -n3
#SBATCH -o 403_64MB.out
#SBATCH -J 403.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 ~/firstests/403/403 64000000

exit 0