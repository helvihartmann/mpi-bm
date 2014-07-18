#!/bin/bash
echo
#not iterated over
q=1 #pipeline Depth
START=4
ENDx=4
for x in $(eval echo "{$START..$ENDx}")
do
    touch single.in
    echo "#!/bin/bash " > single.in
    echo "" >> single.in
    echo "#SBATCH --nodes="$x >> single.in
    echo "#SBATCH --job-name="$x"gegen"$x >> single.in
    echo "#SBATCH --output="$x"gegen"$x"_x"$x"_q"$q"_n"$n".out" >> single.in
    echo "#SBATCH --ntasks-per-node=2" >> single.in
    echo "#SBATCH --nodelist=node0" >> single.in
    echo "#SBATCH --distribution=cyclic" >> single.in
    echo "" >> single.in
    echo "" >> single.in
    echo "mpirun --mca btl_openib_if_include mlx4_0 build/multinodes -o 2 -b 17179869184 -m 2 -q" $q >> single.in
    echo "" >> single.in
    echo "exit 0" >> single.in
    sbatch single.in
    echo "number of root processes x: " $x
    echo "number of nodes with each hosting two processes: " $x
done
exit 0
#output: $m$m"nodes"$x"_"$p"_"$n".out"
