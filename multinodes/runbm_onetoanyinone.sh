#!/bin/bash

q=0 #nature of pipe
START=2
ENDs=3
i=2
o=2
m=2
w=200
a=16
e=18
#b=8589934592
b=4194304
node="dual"

#---------------------------------------------------------
n=$((2*ENDs))
touch single.in
echo "#!/bin/bash " > single.in
echo "" >> single.in
echo "#SBATCH --nodes="$n >> single.in
echo "#SBATCH --constraint="$node >> single.in
echo "#SBATCH --job-name="$i"_"$n >> single.in
echo "#SBATCH --ntasks-per-node=1" >> single.in
echo "" >> single.in
echo "" >> single.in

for s in $(eval echo "{$START..$ENDs}")
do
    r=$s
    echo "mpirun --mca btl_openib_if_include mlx4_0 build/multinodes -i "$i" -w "$w" -e "$e" -o "$o" -q" $q "-s" $s "-r" $r "-b" $b ">> 55nodes_i"$i"_s"$s"_n"$n"_"$node".out" >> single.in
    echo "" >> single.in
    echo "number of senders: " $s
    echo "number of processes n: " $n
done
    echo "multipleib_write_bw.sh" $s $n ">> 55nodes_i"$i"_n"$n"_"$node".log" >> single.in
    echo "exit 0" >> single.in
    cat single.in
    sbatch single.in

exit 0
