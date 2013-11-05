#!/bin/bash

#Submit this script with: sbatch thefilename


#SBATCH -N2 -n2
#SBATCH -o 402_20GB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 20000000000

exit 0
