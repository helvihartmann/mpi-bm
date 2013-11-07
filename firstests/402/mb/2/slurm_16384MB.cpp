#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 402_16384MB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 16384000000

exit 0