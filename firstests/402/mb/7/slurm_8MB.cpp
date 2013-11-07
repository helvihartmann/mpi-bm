#!/bin/bash

#SBATCH -N7 -n7
#SBATCH -o 402_8MB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 8000000

exit 0