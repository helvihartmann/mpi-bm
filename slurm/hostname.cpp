#!/bin/bash

#Submit this script with: sbatch thefilename

#SBATCH -N1 -n1
#SBATCH -J hostname

hostname

exit 0
