#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 402_2048MB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 2048000000

exit 0