#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 502_2048kB.out
#SBATCH -J 502.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 502 2048000

exit 0