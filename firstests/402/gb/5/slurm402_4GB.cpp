#!/bin/bash

#Submit this script with: sbatch thefilename


#SBATCH -N5 -n5
#SBATCH -o 402_4GB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 4000000000

exit 0
