#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 500_16MB.out
#SBATCH -J 500.cpp

mpirun 500 16000000

exit 0