#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 503_16384B.out
#SBATCH -J 503.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 503 16384 39062

exit 0