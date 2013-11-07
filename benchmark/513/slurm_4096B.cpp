#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 513_4096B.out
#SBATCH -J 513.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 513 4096 156250

exit 0