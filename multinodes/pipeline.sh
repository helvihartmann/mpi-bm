#!/bin/bash
echo

p=1
for i in {7..10}
    do

    touch single.in
    echo "#!/bin/bash " > single.in
    echo "" >> single.in
    echo "#SBATCH --nodes=2" >> single.in
    echo "#SBATCH --job-name=i"$i"_p"$p >> single.in
    echo "#SBATCH --output=i"$i"_p"$p".out" >> single.in
    echo "#SBATCH --ntasks-per-node=1" >> single.in
    echo "" >> single.in
    echo "" >> single.in
    echo "mpirun --mca btl_openib_if_include mlx4_0 --mca btl_openib_eager_limit 136 build/multinodes -i "$i" -p "$p" -o 5" >> single.in
    echo "" >> single.in
    echo "exit 0" >> single.in
    sbatch single.in
done
exit 0
