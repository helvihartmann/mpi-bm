#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 504_512B.out
#SBATCH -J 504.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 504 512 1250000

exit 0