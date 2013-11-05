#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 500_64MB.out
#SBATCH -J 500.cpp

mpirun 500 64000000

exit 0