#!/bin/bash
echo

q=0
m=2
for i in 6000 7000 8000 9000 10000 20000 30000 40000
    do
    touch single.in
    echo "#!/bin/bash " > single.in
    echo "" >> single.in
    echo "#SBATCH --nodes=3" >> single.in
    echo "#SBATCH --job-name=i"$i"m_q"$q"_m"$m >> single.in
    echo "#SBATCH --output=i"$i"m_q"$q"_m"$m".out" >> single.in
    echo "#SBATCH --ntasks-per-node="$m >> single.in
    echo "#SBATCH --nodelist=node0" >> single.in
    echo "" >> single.in
    echo "" >> single.in
    echo "mpirun --mca btl_openib_if_include mlx4_0 build/multinodes -b 17179869184 -o 3 -i "$i" -m "$m" " >> single.in
    echo "" >> single.in
    echo "exit 0" >> single.in
    sbatch single.in
done
exit 0
