#!/bin/bash

#SBATCH -N6 -n6
#SBATCH -o 402_4096MB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 4096000000

exit 0