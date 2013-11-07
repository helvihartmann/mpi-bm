#!/bin/bash

#Submit this script with: sbatch thefilename

#SBATCH -N5 -n10 # N nodes; n tasks related to one cpu
#SBATCH -J hostname

hostname

mpirun 400
exit 0
