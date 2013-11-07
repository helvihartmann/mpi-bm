#!/bin/bash

#Submit this script with: sbatch thefilename


#SBATCH -N5 -n10
#SBATCH -o 402_1GB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 1000000000

exit 0
