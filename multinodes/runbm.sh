#!/bin/bash

q=0 #nature of pipe
START=4
END=4#make it always even
stepsize=10
i=2
o=2
w=200
a=16
e=21
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

    for m in 1 2 #to test both modes, some nodes are sender some are receiver (m1) or all nodes send and receive (m2)
    do

        s=$(((n/2)*m))
        echo "multipleib_write_bw.sh $s $END | tee 55nodes_s"$s"_n"$END"_m1.log" >> single.in
        echo "mpirun --npernode "$m" --mca btl_openib_if_include mlx4_0 build/multinodes -m 1 -i "$i" -w "$w" -e "$e" -o "$o" -q" $q "-s" $s "-r" $s "-b" $b ">> 55nodes_m"$m"_s"$s"_n"$END"_"$node".out" >> single.in
        echo "" >> single.in
    done

    echo "number of nodes: " $END
done
    echo "testib.sh "$((END/2))" "$END" >> 55nodesindependent_m1.log" >> single.in
    echo "testib.sh "$END" "$END" >> 55nodesindependent_m2.log" >> single.in
    echo "nodecheck.sh >> nodecheck.log" >> single.in
    echo "exit 0" >> single.in
    cat single.in
    sbatch single.in

exit 0
