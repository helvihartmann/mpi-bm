#!/bin/bash

#Submit this script with: sbatch thefilename


#SBATCH -N3 -n3
#SBATCH -o 402_16GB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 16000000000

exit 0
