#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 501_128kB.out
#SBATCH -J 501.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 501 128000

exit 0