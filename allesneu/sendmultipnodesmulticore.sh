#!/bin/bash
echo
#not iterated over
q=0 #pipeline Depth
START=2
ENDx=2
for x in $(eval echo "{$START..$ENDx}")
do
    for i in 6000 7000 8000 9000 10000 20000 30000 40000
    do
        touch single.in
        echo "#!/bin/bash " > single.in
        echo "" >> single.in
        echo "#SBATCH --nodes="$x >> single.in
        echo "#SBATCH --job-name="$x"vs"$x"i"$i >> single.in
        echo "#SBATCH --output="$x"gegen"$x"_s"$x"_q"$q"_i"$i"m.out" >> single.in
        echo "#SBATCH --ntasks-per-node=2" >> single.in
        echo "#SBATCH --nodelist=node0" >> single.in
        echo "#SBATCH --distribution=cyclic" >> single.in
        echo "" >> single.in
        echo "" >> single.in
        echo "mpirun --mca btl_openib_if_include mlx4_0 build/multinodes -o 3 -i "$i" -b 17179869184 -m 2 -w 1000 -q" $q >> single.in
        echo "" >> single.in
        echo "exit 0" >> single.in
        sbatch single.in
        echo "number of root processes x: " $x
        echo "number of nodes with each hosting two processes: " $x
    done
done
exit 0
#output: $m$m"nodes"$x"_"$p"_"$n".out"
