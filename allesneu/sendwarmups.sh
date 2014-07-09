#!/bin/bash
echo

p=8

for i in 8000 9000 10000 11000 12000 13000 14000 15000
    do
    for w in {1..33}
        do
        w=$((w*10))
        touch single.in
        echo "#!/bin/bash " > single.in
        echo "" >> single.in
        echo "#SBATCH --nodes=2" >> single.in
        echo "#SBATCH --job-name=i"$i"m_w"$w"_p"$p >> single.in
        echo "#SBATCH --output=i"$i"m_w"$w"_p"$p".out" >> single.in
        echo "#SBATCH --ntasks-per-node=1" >> single.in
        echo "#SBATCH --nodelist=node0" >> single.in
        echo "" >> single.in
        echo "" >> single.in
        echo "mpirun --mca btl_openib_if_include mlx4_0 build/multinodes -a 14 -e 14 -b 8589934592 -i "$i" -p "$p" -w "$w" -o 3" >> single.in
        echo "" >> single.in
        echo "exit 0" >> single.in
        sbatch single.in
#echo "$i" -p "$p" -w "$w"
    done
done
exit 0
