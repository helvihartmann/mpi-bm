#!/bin/bash

#Submit this script with: sbatch thefilename


#SBATCH -N8 -n8
#SBATCH -o 402_9.out,i
#SBATCH -J 402.cpp

mpirun 402 1000000000

exit 0
