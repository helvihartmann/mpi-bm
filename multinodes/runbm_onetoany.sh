#!/bin/bash

q=0 #nature of pipe
START=3
ENDs=8
ENDn=4
i=2
o=2
m=2
w=200
e=16
b=8589934592
node="quad"
for s in $(eval echo "{$START..$ENDs}")
do
    if [ $ENDn -lt $((s+s)) ]; then
        echo "Warning ENDn not big enough. automatically set to 2s"
        ENDn=$((s+s))
    fi

    for n in $(eval echo "{$((s+s))..$ENDn}")
    do
        touch single.in
        echo "#!/bin/bash " > single.in
        echo "" >> single.in
        echo "#SBATCH --nodes="$n >> single.in
        echo "#SBATCH --constraint="$node >> single.in
        echo "#SBATCH --job-name="$i"_"$s"_"$n >> single.in
        echo "#SBATCH --output=55nodes_i"$i"_s"$s"_n"$n"_"$node".out" >> single.in
        echo "#SBATCH --ntasks-per-node=1" >> single.in
        echo "" >> single.in
        echo "" >> single.in
        echo "mpirun --mca btl_openib_if_include mlx4_0 build/multinodes -i "$i" -w "$w" -e "$e" -o "$o" -q" $q "-s" $s "-b" $b >> single.in
        echo "" >> single.in
        echo "testib.sh" $s $n >> single.in
        echo "exit 0" >> single.in
        sbatch single.in
        echo "number of root processes x: " $s
        echo "number of processes n: " $n
    done
done
exit 0