#!/bin/bash
echo

p=2
m=5;
for i in {1..7}
    do
    echo $p

    touch single.in
    echo "#!/bin/bash " > single.in
#sbatch pingpongSsend.in
    echo "" >> single.in
    echo "#SBATCH --nodes="$p >> single.in
    echo "#SBATCH --job-name=nodes"$p >> single.in
    echo "#SBATCH --output="$m$m"nodes8_"$p".out" >> single.in
    echo "#SBATCH --ntasks-per-node=1" >> single.in
    echo "" >> single.in
    echo "" >> single.in
    echo "mpirun --mca btl_openib_if_include mlx4_0 build/pingpong -s "$m" -r "$m " -o 2 -e 10000000000 -n 8" >> single.in
    echo "" >> single.in
    echo "exit 0" >> single.in
    sbatch single.in
    p=$((p + 1))
done
exit 0
