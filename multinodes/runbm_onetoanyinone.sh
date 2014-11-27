#!/bin/bash

q=0 #nature of pipe
START=10
ENDs=30
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



for s in $(seq "$START" 10 "$ENDs")
do
    r=$s
    echo "multipleib_write_bw.sh $s $((2*s)) | tee 55nodes_s"$s"_n"$((2*s))".log" >> single.in
    echo "mpirun --mca btl_openib_if_include mlx4_0 build/multinodes -i "$i" -w "$w" -e "$e" -o "$o" -q" $q "-s" $s "-r" $r "-b" $b ">> 55nodes_i"$i"_s"$s"_n"$n"_"$node".out" >> single.in
    echo "" >> single.in
    echo "number of senders: " $s
    echo "number of processes n: " $n
done
    echo "testib.sh "$ENDs" "$n" >> 55nodesindependent.log" >> single.in
    echo "exit 0" >> single.in
    cat single.in
    sbatch single.in

exit 0
