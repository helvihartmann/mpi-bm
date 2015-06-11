#!/bin/bash 

#SBATCH --nodes=12
#SBATCH --job-name=10000_24



/opt/openmpi185/bin/mpirun --npernode 2 --mca btl_openib_if_include mlx4_0 ../build/lse-mpi-bm -i 10000 -w 1000 -e 22 -o 2 -s 12 -r 12 -m 2 -b 17179869184 > test_m2_n12.out
exit 0
