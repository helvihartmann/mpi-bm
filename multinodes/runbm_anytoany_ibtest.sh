#!/bin/bash
echo
#not iterated over
START=2
ENDx=3
i=2000
o=3
m=2
w=600
Nodelist=(node0 node1 node2 node3 node4 node5 node6 node7)

echo "logfile" > "i"$i"_m"_$m"_w"$w".log"
for j in $(seq 0 $((ENDx-2)))
do
#Nodelist+=("node"$j)
echo "testing ib connection for" ${Nodelist[$j]} ${Nodelist[$((j+1))]} >> "i"$i"_m"_$m"_w"$w".log"
./myib_write_bw.sh ${Nodelist[$j]} ${Nodelist[$((j+1))]} >> "i"$i"_m"_$m"_w"$w".log"
echo " "
done
#echo "testing ib connection for" ${Nodelist[$((j+1))]} ${Nodelist[0]}
#./myib_write_bw.sh ${Nodelist[$((j+1))]} ${Nodelist[0]}

rm slurmib_write_bw.sh

for s in $(eval echo "{$START..$ENDx}")
do
    echo "number of nodes " $s
    echo " "
    echo "writing file for benchmark"

#writing script
    touch single.in
    echo "#!/bin/bash " > single.in
    echo "" >> single.in
    echo "#SBATCH --nodes="$s >> single.in
    echo "#SBATCH --job-name="$s"vs"$s >> single.in
    echo "#SBATCH --output="$s"gegen"$s"_s"$s"_i"$i"g.out" >> single.in
    echo "#SBATCH --ntasks-per-node="$m >> single.in
    echo "#SBATCH --nodelist=node0" >> single.in
    echo "#SBATCH --distribution=cyclic" >> single.in
    echo "" >> single.in
    echo "" >> single.in
    echo "mpirun --mca btl_openib_if_include mlx4_0 build/multinodes -o "$o" -i "$i" -b 8589934592 -m "$m" -w" $w >> single.in
    echo "" >> single.in
    echo "exit 0" >> single.in

    echo "sending benchmark via slurm"
    echo " "
#sending script
#sbatch single.in

done
exit 0