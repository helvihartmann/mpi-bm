#!/bin/bash
echo

p=1
for i in {1..30}
    do
    echo $p
    p=$((p * 2 ))
    touch single.in
    echo "#!/bin/bash " > single.in
#sbatch pingpongSsend.in
    echo "" >> single.in
    echo "#SBATCH --nodes=2" >> single.in
    echo "#SBATCH --job-name=single"$p >> single.in
    echo "#SBATCH --output=s"$p".out" >> single.in
    echo "#SBATCH --ntasks-per-node=1" >> single.in
    echo "" >> single.in
    echo "" >> single.in
    echo "mpirun --mca orte_base_help_aggregate 0 --mca btl_openib_if_include mlx4_0 build/pingpong -m 2 -o 10 -a "$p" -e "$((p+1)) >> single.in
    echo "" >> single.in
    echo "exit 0" >> single.in
    sbatch single.in
done
exit 0
