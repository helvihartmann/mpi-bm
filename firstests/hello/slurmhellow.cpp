#!/bin/bash

#Submit this script with: sbatch thefilename

#SBATCH -J slurmhellow 

hostname
mpiexec ./hellow

exit 0
