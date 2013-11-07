#!/bin/bash

#SBATCH -N6 -n6
#SBATCH -o 402_1024MB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 1024000000

exit 0