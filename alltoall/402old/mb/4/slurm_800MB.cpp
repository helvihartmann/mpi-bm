#!/bin/bash

#SBATCH -N4 -n4
#SBATCH -o 402_800MB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 800000000

exit 0