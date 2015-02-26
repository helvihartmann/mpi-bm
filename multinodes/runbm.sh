#!/bin/bash

q=0 #nature of pipe
START=4
END=8 #make it always even
stepsize=2
i=2000
o=2
w=200
a=16
e=21
b=8589934592
#b=4194304
node="dual"

#---------------------------------------------------------

touch single.in
echo "#!/bin/bash " > single.in
echo "" >> single.in
echo "#SBATCH --nodes="$END >> single.in
#echo "#SBATCH --constraint="$node >> single.in
echo "#SBATCH --job-name="$i"_"$END >> single.in
echo "" >> single.in
echo "" >> single.in

#for the first benchmark have single process on each node and test maximum performance
echo "testib_parallel.sh "$((START/2))" "$START" | tee 55nodes_s"$s"_n"$END"_m"$m".log" >> single.in
echo "mpirun --npernode 1 --mca btl_openib_if_include mlx4_0 build/multinodes -m 1 -i "$i" -w "$w" -e "$e" -o "$o" -q" $q "-s" $s "-r" $s "-b" $b ">> 55nodes_m1_s"$s"_n"$END"_"$node".out" >> single.in
echo "" >> single.in


for n in $(seq "$START" "$stepsize" "$END")
do


    echo "testib_parallel.sh "$n" "$n" | tee 55nodes_s"$s"_n"$END"_m"$m".log" >> single.in
    echo "mpirun --npernode 2 --mca btl_openib_if_include mlx4_0 build/multinodes -m 2 -i "$i" -w "$w" -e "$e" -o "$o" -q" $q "-s" $n "-r" $s "-b" $b ">> 55nodes_m2_s"$n"_n"$END"_"$node".out" >> single.in
    echo "" >> single.in

    echo "number of nodes: " $END
done
    echo "testib.sh "$END" "$END" >> 55nodesindependent_m2.log" >> single.in
    echo "nodecheck.sh >> nodecheck.log" >> single.in
    echo "collectresult.sh" >> single.in
    echo "exit 0" >> single.in
    cat single.in
    sbatch single.in

exit 0
