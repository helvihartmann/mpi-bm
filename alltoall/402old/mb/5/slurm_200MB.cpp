#!/bin/bash

#SBATCH -N5 -n5
#SBATCH -o 402_200MB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 200000000

exit 0