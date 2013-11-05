#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 500_4096MB.out
#SBATCH -J 500.cpp

mpirun 500 4096000000

exit 0