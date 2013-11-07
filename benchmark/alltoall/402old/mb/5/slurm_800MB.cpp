#!/bin/bash

#SBATCH -N5 -n5
#SBATCH -o 402_800MB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 800000000

exit 0