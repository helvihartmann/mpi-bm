#!/bin/bash
echo
#not iterated over
q=0 #pipeline Depth
START=1
ENDx=8
for x in $(eval echo "{$START..$ENDx}")
do
    touch single.in
    echo "#!/bin/bash " > single.in
    echo "" >> single.in
    echo "#SBATCH --nodes="$x >> single.in
    echo "#SBATCH --job-name="$x"gegen"$x >> single.in
    echo "#SBATCH --output="$x"gegen"$x"_x"$x"_q"$q"_n"$n".out" >> single.in
    echo "#SBATCH --ntasks-per-node=2" >> single.in
    echo "#SBATCH --nodelist=node1" >> single.in
    echo "#SBATCH --distribution=cyclic" >> single.in
    echo "" >> single.in
    echo "" >> single.in
    echo "mpirun --mca btl_openib_if_include mlx4_0 build/multinodes -q 0 -o 2 -b 8589934592 -i 1000 -w 300 -q" $q "-x" $x >> single.in
    echo "" >> single.in
    echo "exit 0" >> single.in
    sbatch single.in
    echo "number of root processes x: " $x
    echo "number of nodes with each hosting two processes: " $x
done
exit 0
#output: $m$m"nodes"$x"_"$p"_"$n".out"
