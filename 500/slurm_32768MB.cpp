#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 500_32768MB.out
#SBATCH -J 500.cpp

mpirun 500 32768000000

exit 0