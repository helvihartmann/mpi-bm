#!/bin/bash

#SBATCH -N3 -n3
#SBATCH -o 402_128MB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 128000000

exit 0