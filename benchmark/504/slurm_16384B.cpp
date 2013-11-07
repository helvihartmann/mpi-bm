#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 504_16384B.out
#SBATCH -J 504.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 504 16384 39062

exit 0