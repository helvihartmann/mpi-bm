#!/bin/bash

#Submit this script with: sbatch thefilename


#SBATCH -N8 -n8
#SBATCH -o 402_5.out,i
#SBATCH -J 402.cpp

mpirun 402 100000

exit 0
