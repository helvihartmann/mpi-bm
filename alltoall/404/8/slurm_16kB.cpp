#!/bin/bash

#SBATCH -N8 -n8
#SBATCH -o 404_16kB.out
#SBATCH -J 404.cpp

mpirun --mca btl_openib_warn_no_device_params_found 0 ~/firstests/404/404 16000

exit 0