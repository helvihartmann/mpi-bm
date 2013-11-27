#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 523_128B.out
#SBATCH -J 523.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 523 128 5000000

exit 0