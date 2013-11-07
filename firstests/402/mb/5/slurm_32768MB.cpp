#!/bin/bash

#SBATCH -N5 -n5
#SBATCH -o 402_32768MB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 32768000000

exit 0