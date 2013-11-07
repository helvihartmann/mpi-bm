#!/bin/bash

#Submit this script with: sbatch thefilename


#SBATCH -N8 -n8
#SBATCH -o 402_8GB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 8000000000

exit 0
