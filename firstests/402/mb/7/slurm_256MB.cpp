#!/bin/bash

#SBATCH -N7 -n7
#SBATCH -o 402_256MB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 256000000

exit 0