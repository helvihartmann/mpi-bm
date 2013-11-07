#!/bin/bash

#Submit this script with: sbatch thefilename


#SBATCH -N4 -n4
#SBATCH -o 402_4GB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 4,000,000,000

exit 0
