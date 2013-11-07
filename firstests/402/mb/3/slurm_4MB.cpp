#!/bin/bash

#SBATCH -N3 -n3
#SBATCH -o 402_4MB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 4000000

exit 0