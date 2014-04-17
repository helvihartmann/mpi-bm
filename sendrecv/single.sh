#!/bin/bash
echo

#iterated over
n=2 #number of processes
x=1 #number of root processes
#not iterated over
m=5 #mode
p=8 #pipeline Depth
START=1
END=$((n - 1))
for i in {1..7}
do
    for j in $(eval echo "{$START..$END}")
    do

        touch single.in
        echo "#!/bin/bash " > single.in
    #sbatch pingpongSsend.in
        echo "" >> single.in
        echo "#SBATCH --nodes="$n >> single.in
        echo "#SBATCH --job-name="$m$m"_"$x"_"$p"_"$n >> single.in
        echo "#SBATCH --output="$m$m"nodes_x"$x"_p"$p"_n"$n".out" >> single.in
        echo "#SBATCH --ntasks-per-node=1" >> single.in
        echo "" >> single.in
        echo "" >> single.in
        echo "mpirun --mca btl_openib_if_include mlx4_0 --mca btl_openib_eager_limit 128 build/pingpong -s "$m" -r "$m "-o 2 -e 10000000 -p" $p "-x" $x >> single.in
        echo "" >> single.in
        echo "exit 0" >> single.in
        sbatch single.in

        x=$((x + 1))
    done
    n=$((n + 1))
    x=1
    END=$((n - 1))
done
exit 0
#output: $m$m"nodes"$x"_"$p"_"$n".out"