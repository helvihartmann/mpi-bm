#!/bin/bash

#SBATCH -N6 -n6
#SBATCH -o 402_8192MB.out
#SBATCH -J 402.cpp

mpirun ~/firstests/402/402 8192000000

exit 0