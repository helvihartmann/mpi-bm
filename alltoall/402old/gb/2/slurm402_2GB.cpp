#!/bin/bash

#Submit this script with: sbatch thefilename


#SBATCH -N2 -n2
#SBATCH -o 402_2GB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 2,000,000,000

exit 0
