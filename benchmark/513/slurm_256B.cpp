#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 513_256B.out
#SBATCH -J 513.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 513 256 2500000

exit 0