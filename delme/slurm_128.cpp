#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 503.out
#SBATCH -J 503.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 503 128

exit 0
