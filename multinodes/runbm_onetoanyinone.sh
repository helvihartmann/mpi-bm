#!/bin/bash

q=0 #nature of pipe
START=4
END=4
stepsize=10
i=2
o=2
w=200
a=16
e=18
#b=8589934592
b=4194304
node="dual"

#---------------------------------------------------------

touch single.in
echo "#!/bin/bash " > single.in
echo "" >> single.in
echo "#SBATCH --nodes="$END >> single.in
echo "#SBATCH --constraint="$node >> single.in
echo "#SBATCH --job-name="$i"_"$END >> single.in
echo "" >> single.in
echo "" >> single.in



for n in $(seq "$START" "$stepsize" "$END")
do
    sm1=$((n/2))
    sm2=$n
    pm1=$n
    pm2=$((2*n))
    numberprocesses=$((2*s))


    echo "#multipleib_write_bw.sh $s $numberprocesses | tee 55nodes_s"$s"_n"$numberprocesses"_m1.log" >> single.in
    echo "mpirun -n "$pm1" --mca btl_openib_if_include mlx4_0 build/multinodes -m 1 -i "$i" -w "$w" -e "$e" -o "$o" -q" $q "-s" $sm1 "-r" $sm1 "-b" $b ">> 55nodes_m1_s"$s"_n"$END"_"$node".out" >> single.in
    echo "" >> single.in

    echo "#multipleib_write_bw.sh $numberprocesses $numberprocesses | tee 55nodes_s"$s"_n"$numberprocesses"_m2.log" >> single.in
    echo "mpirun -n "$pm2" --mca btl_openib_if_include mlx4_0 build/multinodes -m 2 -i "$i" -w "$w" -e "$e" -o "$o" -q" $q "-s" $sm2 "-b" $b ">> 55nodes_m2_s"$s"_n"$END"_"$node".out" >> single.in
    echo "" >> single.in
    echo "number of nodes: " $END
done
    echo "testib.sh "$sm1" "$END" >> 55nodesindependent_m1.log" >> single.in
    echo "testib.sh "$END" "$END" >> 55nodesindependent_m2.log" >> single.in
    echo "exit 0" >> single.in
    cat single.in
    sbatch single.in

exit 0
