#!/bin/bash

#SBATCH -N3 -n3
#SBATCH -o 402_16384MB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 16384000000

exit 0