#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 500_1MB.out
#SBATCH -J 500.cpp

mpirun 500 1000000

exit 0
