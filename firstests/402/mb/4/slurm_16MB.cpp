#!/bin/bash

#SBATCH -N4 -n4
#SBATCH -o 402_16MB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 16000000

exit 0