#!/bin/bash

#SBATCH -N5 -n5
#SBATCH -o 402_400MB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 400000000

exit 0