#!/bin/bash
echo
#not iterated over
q=0 #pipeline Depth
START=3
ENDx=3
for x in $(eval echo "{$START..$ENDx}")
do
    for w in 200 300 400 500 600 700 800 900 1000 1100 1200 1300 1400 1500 1600 1700 1800 1900 2000
    do
        touch single.in
        echo "#!/bin/bash " > single.in
        echo "" >> single.in
        echo "#SBATCH --nodes="$x >> single.in
        echo "#SBATCH --job-name="$x"vs"$x"w"$w >> single.in
        echo "#SBATCH --output="$x"gegen"$x"_s"$x"_q"$q"_w"$w".out" >> single.in
        echo "#SBATCH --ntasks-per-node=2" >> single.in
        echo "#SBATCH --nodelist=node0" >> single.in
        echo "#SBATCH --distribution=cyclic" >> single.in
        echo "" >> single.in
        echo "" >> single.in
        echo "mpirun --mca btl_openib_if_include mlx4_0 build/multinodes -o 3 -i 6000 -b 8589934592 -m 2 -w "$w" -q" $q >> single.in
        echo "" >> single.in
        echo "exit 0" >> single.in
        sbatch single.in
        echo "number of root processes x: " $x
        echo "number of nodes with each hosting two processes: " $x
    done
done
exit 0
#output: $m$m"nodes"$x"_"$p"_"$n".out"
