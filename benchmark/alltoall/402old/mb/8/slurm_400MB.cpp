#!/bin/bash

#SBATCH -N8 -n8
#SBATCH -o 402_400MB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 400000000

exit 0