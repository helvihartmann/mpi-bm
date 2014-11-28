#!/bin/bash

START=2
ENDx=3
i=2
o=3
m=2
w=400
e=16


#running mpi benchmark
for s in $(eval echo "{$START..$ENDx}")
do
    echo "number of nodes " $s
    echo " "
    echo "writing file for benchmark"

    #writing script
    touch single.in
    echo "#!/bin/bash " > single.in
    echo "" >> single.in
    echo "#SBATCH --nodes="$s >> single.in
    echo "#SBATCH --job-name="$s"vs"$s >> single.in
    echo "#SBATCH --output="$s"gegen"$s"_s"$s"_i"$i"g.out" >> single.in
    echo "#SBATCH --ntasks-per-node="$m >> single.in
    echo "#SBATCH --distribution=cyclic" >> single.in
    echo "" >> single.in
    echo "" >> single.in
    echo "mpirun --mca btl_openib_if_include mlx4_0 build/multinodes -o "$o" -i "$i" -b 8589934592 -m "$m" -w" $w "-e" $e >> single.in
    echo "" >> single.in
    echo "testib.sh" >> single.in
    echo "exit 0" >> single.in

    #sending script
    echo "sending benchmark via slurm"
    echo " "
    sbatch single.in

done
exit 0
