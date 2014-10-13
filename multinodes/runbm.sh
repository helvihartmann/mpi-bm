#!/bin/bash

i=2
o=3
m=2
w=200
s=1#unnecessary for m2

#other options a, e, r, f, p, q, t,x put build/multinodes -h for more infromation
echo "  ,.,  "
echo "  (o,o)"
echo " <    >"
echo "  _'""'__"
echo " "

rm *.out

touch single.in
echo "#!/bin/bash " > single.in
echo "" >> single.in
echo "#SBATCH --nodes="$nodes >> single.in
echo "#SBATCH --job-name=s"$s"_i"$i"g_m"$m >> single.in
echo "#SBATCH --output=s"$s"_i"$i"g_m"$m".out" >> single.in
echo "#SBATCH --ntasks-per-node="$m >> single.in
echo "#SBATCH --nodelist=node0" >> single.in
echo "#SBATCH --distribution=cyclic" >> single.in
echo "" >> single.in
echo "" >> single.in

#include options here if necessary
echo "mpirun --mca btl_openib_if_include mlx4_0 build/multinodes -o "$o" -i "$i" -b 8589934592 -m "$m" -w" $w >> single.in
echo "" >> single.in
echo "testib.sh" >> single.in
echo "exit 0" >> single.in


#ib_write_bw.sh
sbatch single.in

sleep 2
echo "starting script"
tail -f *.out
exit 0
