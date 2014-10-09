#!/bin/bash
echo
#not iterated over
q=0 #nature of pipe
START=1
ENDx=1
ENDn=2
for s in $(eval echo "{$START..$ENDx}")
do
    if [ $ENDn -lt $ENDx ]; then
        echo "Warning ENDn not big enough. automatically set to ENDx+1"
        ENDn=$((ENDx+1))
    fi

    for n in $(eval echo "{$((s+1))..$ENDn}")
    do
        touch single.in
        echo "#!/bin/bash " > single.in
        echo "" >> single.in
        echo "#SBATCH --nodes="$n >> single.in
        echo "#SBATCH --job-name="$s"_"$q"_"$n >> single.in
        echo "#SBATCH --output=55nodes_s"$s"_q"$q"_n"$n".out" >> single.in
        echo "#SBATCH --ntasks-per-node=1" >> single.in
        echo "" >> single.in
        echo "" >> single.in
        echo "mpirun --mca btl_openib_if_include mlx4_0 build/multinodes -i 180000 -w 1000 -e 18 -o 2 -q" $q "-s" $s >> single.in
        echo "" >> single.in
        echo "exit 0" >> single.in
        sbatch single.in
        echo "number of root processes x: " $s
        echo "number of processes n: " $n
    done
done
exit 0