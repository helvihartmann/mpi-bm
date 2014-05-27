#!/bin/bash
echo
#not iterated over
p=8 #pipeline Depth
START=1
ENDx=1
ENDn=2
for x in $(eval echo "{$START..$ENDx}")
do
    if [ $ENDn -lt $ENDx ]; then
        echo "Warning ENDn not big enough. automatically set to ENDx+1"
        ENDn=$((ENDx+1))
    fi

    for n in $(eval echo "{$((x+1))..$ENDn}")
    do
        touch single.in
        echo "#!/bin/bash " > single.in
        echo "" >> single.in
        echo "#SBATCH --nodes="$n >> single.in
        echo "#SBATCH --job-name=_"$x"_"$p"_"$n >> single.in
        echo "#SBATCH --output=nodes_x"$x"_p"$p"_n"$n".out" >> single.in
        echo "#SBATCH --ntasks-per-node=1" >> single.in
        echo "#SBATCH --nodelist=node0" >> single.in
        echo "" >> single.in
        echo "" >> single.in
        echo "mpirun --mca btl_openib_if_include mlx4_0 --mca btl_openib_eager_limit 136 build/multinodes -o 2 -w 10 -e 20 -p" $p "-x" $x >> single.in
        echo "" >> single.in
        echo "exit 0" >> single.in
        sbatch single.in
        echo "number of root processes x: " $x
        echo "number of processes n: " $n
    done
done
exit 0
#output: $m$m"nodes"$x"_"$p"_"$n".out"