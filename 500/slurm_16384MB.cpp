#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 500_16384MB.out
#SBATCH -J 500.cpp

mpirun 500 16384000000

exit 0