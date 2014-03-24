#!/bin/bash
echo

p=1
for i in {2..10}
    do
    echo $p

    touch single.in
    echo "#!/bin/bash " > single.in
#sbatch pingpongSsend.in
    echo "" >> single.in
    echo "#SBATCH --nodes=2" >> single.in
    echo "#SBATCH --job-name=calls"$p >> single.in
    echo "#SBATCH --output=55calls"$p".out" >> single.in
    echo "#SBATCH --ntasks-per-node=1" >> single.in
    echo "" >> single.in
    echo "" >> single.in
    echo "mpirun --mca btl_openib_if_include mlx4_0 build/pingpong -s 5 -r 5 -o 2 -e 10000000000 -n" $p >> single.in
    echo "" >> single.in
    echo "exit 0" >> single.in
    sbatch single.in
    p=$((p * 2 ))
done
exit 0
