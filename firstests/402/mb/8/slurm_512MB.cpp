#!/bin/bash

#SBATCH -N8 -n8
#SBATCH -o 402_512MB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 512000000

exit 0