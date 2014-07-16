#!/bin/bash
echo
#not iterated over
q=1 #nature of pipe
START=3
ENDx=7
ENDn=8
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
        echo "#SBATCH --job-name="$x"_"$q"_"$n >> single.in
        echo "#SBATCH --output=55nodes_x"$x"_q"$q"_n"$n".out" >> single.in
        echo "#SBATCH --ntasks-per-node=1" >> single.in
        echo "#SBATCH --nodelist=node0" >> single.in
        echo "" >> single.in
        echo "" >> single.in
        echo "mpirun --mca btl_openib_if_include mlx4_0 build/multinodes -w 190 -o 10 -q" $q "-x" $x >> single.in
        echo "" >> single.in
        echo "exit 0" >> single.in
        sbatch single.in
        echo "number of root processes x: " $x
        echo "number of processes n: " $n
    done
done
exit 0
#output: $m$m"nodes"$x"_"$p"_"$n".out"
