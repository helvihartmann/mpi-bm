#!/bin/bash
echo

p=1
m=5
for i in {2..5}
    do
    echo $p

    touch single.in
    echo "#!/bin/bash " > single.in
#sbatch pingpongSsend.in
    echo "" >> single.in
    echo "#SBATCH --nodes=2" >> single.in
echo "#SBATCH --job-name="$m$m"pipe"$p >> single.in
echo "#SBATCH --output="$m$m"pipe"$p".out" >> single.in
    echo "#SBATCH --ntasks-per-node=1" >> single.in
    echo "" >> single.in
    echo "" >> single.in
    echo "mpirun --mca btl_openib_if_include mlx4_0 --mca btl_openib_eager_limit 136 build/pingpong -s "$m" -r "$m "-o 2 -e 10000000000 -p" $p >> single.in
    echo "" >> single.in
    echo "exit 0" >> single.in
    sbatch single.in
    p=$((p * 2 ))
done
exit 0
