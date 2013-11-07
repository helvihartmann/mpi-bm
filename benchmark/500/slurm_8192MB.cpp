#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 500_8192MB.out
#SBATCH -J 500.cpp

mpirun 500 8192000000

exit 0