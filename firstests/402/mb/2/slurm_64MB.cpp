#!/bin/bash

#SBATCH -N2 -n2
#SBATCH -o 402_64MB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 64000000

exit 0