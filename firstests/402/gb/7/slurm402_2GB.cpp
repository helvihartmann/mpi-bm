#!/bin/bash

#Submit this script with: sbatch thefilename


#SBATCH -N7 -n7
#SBATCH -o 402_2GB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 2000000000

exit 0
