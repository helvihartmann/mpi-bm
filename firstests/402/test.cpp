#!/bin/bash

#Submit this script with: sbatch thefilename


#SBATCH -N2 -n2
#SBATCH -o 402_1GB.out
#SBATCH -J 402.cpp

mpirun 402 1000000000

exit 0
