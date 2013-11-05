#!/bin/bash

#Submit this script with: sbatch thefilename


#SBATCH -N4 -n4
#SBATCH -o 402_10GB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 10,000,000,000

exit 0