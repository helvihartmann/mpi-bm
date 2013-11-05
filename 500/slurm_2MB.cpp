#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 500_2MB.out
#SBATCH -J 500.cpp

mpirun 500 2000000

exit 0