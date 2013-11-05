#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 500_128MB.out
#SBATCH -J 500.cpp

mpirun 500 128000000

exit 0