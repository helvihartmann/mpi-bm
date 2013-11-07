#!/bin/bash

#SBATCH -N5 -n5
#SBATCH -o 402_4096MB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 4096000000

exit 0