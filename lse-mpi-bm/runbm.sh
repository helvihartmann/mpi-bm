#!/bin/bash

q=0 #nature of pipe
START=4
END=24 #make it always even
stepsize=4
i=10000
o=2
w=200
a=16
e=22
#b=8589934592
b=17179869184
node="dual"

#---------------------------------------------------------

touch single.in
echo "#!/bin/bash " > single.in
echo "" >> single.in
echo "#SBATCH --nodes="$END >> single.in
#echo "#SBATCH --constraint="$node >> single.in
echo "#SBATCH --job-name="$i"_"$END >> single.in
echo "#SBATCH --nodefile=nodelist24.txt" >> single.in
echo "" >> single.in
echo "" >> single.in

#for the first benchmark have single process on each node and test maximum performance
#echo "testib_parallel.sh "$((START/2))" "$START" | tee 55nodes_s"$s"_n"$END"_m"$m".log" >> single.in
#echo "mpirun --npernode 1 --mca btl_openib_if_include mlx4_0 build/multinodes -m 1 -i "$i" -w "$w" -e "$e" -o "$o" -q" $q "-s" $((START/2)) "-r" $((START/2)) "-b" $b ">> 55nodes_m1_s"$((START/2))"_n"$END"_"$node".out" >> single.in
echo "" >> single.in


for n in $(seq "$START" "$stepsize" "$END")
do


#echo "testib_parallel.sh "$n" "$n" | tee 55nodes_s"$s"_n"$END"_m"$m".log" >> single.in
    echo "/opt/openmpi185/bin/mpirun --npernode 1 --mca btl_openib_if_include mlx4_0 build/lse-mpi-bm -m 1 -i "$i" -w "$w" -e "$e" -o "$o "-s" $((n/2)) "-r" $((n/2)) "-b" $b ">> 55nodes_m1_s"$((n/2))"_n"$END"_"$node".out" >> single.in

    echo "/opt/openmpi185/bin/mpirun --npernode 2 --mca btl_openib_if_include mlx4_0 build/lse-mpi-bm -m 2 -i "$i" -w "$w" -e "$e" -o "$o "-s" $n "-r" $n "-b" $b ">> 55nodes_m2_s"$n"_n"$END"_"$node".out" >> single.in
    echo "" >> single.in

    echo "number of nodes: " $END
done
    echo "testib_fullindependent.sh "$END" "$END" >> 55nodesindependent_m2.log" >> single.in
    echo "testib_fast.sh "$END" "$END" >> 55nodesindependent_m2fast.log" >> single.in
#echo "nodecheck.sh >> nodecheck.log" >> single.in
    echo "collectresult.sh" >> single.in
    echo "exit 0" >> single.in
    cat single.in
#sbatch single.in

exit 0
