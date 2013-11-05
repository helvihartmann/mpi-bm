#!/bin/bash

#Submit this script with: sbatch thefilename

#SBATCH -o 400.out

#SBATCH -J 400
#SBATCH --clusters=mpp1 
#SBATCH --ntasks=64 #SBATCH --mail-type=end 
#SBATCH --mail-user=xyz@xyz.de 
#SBATCH --export=NONE 
#SBATCH --time=08:00:00 
source /etc/profile.d/modules.sh
cd $OPT_TMP/mydata
srun -n 4 400.cpp
