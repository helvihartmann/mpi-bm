#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 504_262144B.out
#SBATCH -J 504.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 504 262144 2441

exit 0