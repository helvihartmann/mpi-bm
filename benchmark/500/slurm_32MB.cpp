#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 500_32MB.out
#SBATCH -J 500.cpp

mpirun 500 32000000

exit 0