#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 500_8MB.out
#SBATCH -J 500.cpp

mpirun 500 8000000

exit 0