#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 513_2048B.out
#SBATCH -J 513.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 513 2048 312500

exit 0