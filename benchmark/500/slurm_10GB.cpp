#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 500_10GB.out
#SBATCH -J 500.cpp

mpirun 500 10000000000

exit 0
