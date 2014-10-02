#!/bin/bash
echo
#not iterated over
START=2
ENDx=8
for s in $(eval echo "{$START..$ENDx}")
do
    for w in 600
    do
#writing script
        touch single.in
        echo "#!/bin/bash " > single.in
        echo "" >> single.in
        echo "#SBATCH --nodes="$s >> single.in
        echo "#SBATCH --job-name="$s"vs"$s >> single.in
        echo "#SBATCH --output="$s"gegen"$s"_s"$s"_barrier_an_i2g.out" >> single.in
        echo "#SBATCH --ntasks-per-node=2" >> single.in
        echo "#SBATCH --nodelist=node0" >> single.in
        echo "#SBATCH --distribution=cyclic" >> single.in
        echo "" >> single.in
        echo "" >> single.in
        echo "mpirun --mca btl_openib_if_include mlx4_0 build/multinodes -o 3 -i 2000 -b 8589934592 -m 2 -w" $w >> single.in
        echo "" >> single.in
        echo "exit 0" >> single.in

#sending script
        sbatch single.in

        echo "number of root processes x: " $s
        echo "number of nodes with each hosting two processes: " $s
    done
done
exit 0
#output: $m$m"nodes"$x"_"$p"_"$n".out"
