#!/bin/bash

#SBATCH -N7 -n7
#SBATCH -o 402_1024MB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 1024000000

exit 0