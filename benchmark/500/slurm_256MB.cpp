#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 500_256MB.out
#SBATCH -J 500.cpp

mpirun 500 256000000

exit 0