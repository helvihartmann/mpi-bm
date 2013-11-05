#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 500_2048MB.out
#SBATCH -J 500.cpp

mpirun 500 2048000000

exit 0