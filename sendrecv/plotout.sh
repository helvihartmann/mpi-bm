#!/bin/bash
# list-glob.sh: Generating [list] in a for-loop, using "globbing" ...
# Globbing = filename expansion.
echo
START=1
for file in *.out
    do
    ls "$file" # Lists all files in $PWD (current directory).

    my_var=${file%.out}
    echo "creating " "$my_var"".gnuplot ..."

    touch "$my_var".gnuplot
    echo "#!/usr/bin/gnuplot " > "$my_var".gnuplot
    echo " " >> "$my_var".gnuplot
    echo " " >> "$my_var".gnuplot
    echo "set output '"$my_var".pdf'" >> "$my_var".gnuplot
    echo "set terminal pdf" >> "$my_var".gnuplot
    echo "set logscale x 2" >> "$my_var".gnuplot
    echo 'set ylabel "Data rate [MB/s]"' >> "$my_var".gnuplot
    echo "#set grid" >> "$my_var".gnuplot
    echo "set yrange [1:6000]" >> "$my_var".gnuplot
    echo "set format y '%.0e'" >> "$my_var".gnuplot
    echo "set xrange [1:5e9]" >> "$my_var".gnuplot
    echo "set key left" >> "$my_var".gnuplot
    echo 'set xtics ("1B" 1, " " 2, " " 4, "8B" 8," " 16, " " 32, "64B" 64, " " 128, " " 256, "512B" 512, " " 1024, " " 2048, "4kiB" 4096," " 8192, " " 16384, "32kiB" 32768, " " 65536, " " 131072, " " 262144, "512kiB" 524288, " " 1048576, " " 2097152, " " 4194304, "8MiB" 8388608, " " 16777216, " " 33554432, " " 67108864, "128MiB" 134217728," "268435456," " 536870912," " 1073741824,"2GiB"2.147e9," "4.294e9)' >> "$my_var".gnuplot
    echo "set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 5 ps 0.2   # --- blue" >> "$my_var".gnuplot
    echo "set style line 2 lc rgb '#1E90FF' lt 1 lw 1 pt 4 ps 0.4   # --- blue" >> "$my_var".gnuplot
    echo "set style line 3 lc rgb '#9932CC' lt 1 lw 2 pt 5 ps 0.2   # --- darkorchid" >> "$my_var".gnuplot
    echo "set style line 3 lc rgb '#483D8B' lt 1 lw 1 pt 4 ps 0.4   # --- darkslateblue" >> "$my_var".gnuplot
    echo "set style line 5 lc rgb '#1E90FF' lt 1 lw 2 pt 5 ps 0.2   # --- dodgerblue" >> "$my_var".gnuplot
    echo "set style line 6 lc rgb '#4B0082' lt 1 lw 1 pt 4 ps 0.4   # --- indigo" >> "$my_var".gnuplot
    echo "set style line 7 lc rgb '#ADD8E6' lt 1 lw 2 pt 5 ps 0.2   # --- lightblue" >> "$my_var".gnuplot
    echo "set style line 8 lc rgb '#E0FFFF' lt 1 lw 1 pt 4 ps 0.4   # --- lightcyan" >> "$my_var".gnuplot
    echo "set style line 9 lc rgb '#FA8072' lt 1 lw 2 pt 4 ps 0.2   # --- salmon" >> "$my_var".gnuplot
    echo "set style line 10 lc rgb '#2E8B57' lt 1 lw 2 pt 4 ps 0.2   # --- green" >> "$my_var".gnuplot
    echo "set multiplot layout 2,1 rowsfirst" >> "$my_var".gnuplot
    echo "unset key" >> "$my_var".gnuplot
    echo "plot \\" >> "$my_var".gnuplot

    numberprocesses=$(awk '/processes/ {print $3;exit}' $file)
    END=$(($numberprocesses-1))
    for i in $(eval echo "{$START..$END}")
    do
        echo "  'out/"$file"' i 'process" "$i" "printing' using 3:6 w lines ls " "$i" " title '', \\" >> "$my_var".gnuplot
        echo "  'out/"$file"' i 'process" "$i" "printing' using 3:6:7 w yerrorbars ls " "$i" " title 'node 0 -" "$i" "' axes x1y1, \\" >> "$my_var".gnuplot
#echo "  'out/"$file"' i 'data sent to" "$i""' using 3:(\$6/1000000) w lines ls " "$i" " title 'warmup ""$i""', \\" >> "$my_var".gnuplot
    done
    echo "  'out/imb.out' using 1:4 w linespoint ls 9 title 'intel',\\" >> "$my_var".gnuplot
    echo "  'out/ib_rdma_bw.out' using 1:4 w linespoint ls 10 title 'ib_write_bw'" >> "$my_var".gnuplot
    echo "set logscale y 2" >> "$my_var".gnuplot
    echo "set key right bottom" >> "$my_var".gnuplot
    echo 'set xlabel "Package size [B]"' >> "$my_var".gnuplot
    echo "plot \\" >> "$my_var".gnuplot
    for i in $(eval echo "{$START..$END}")
    do
    echo "  'out/"$file"' i 'process" "$i" "printing' using 3:6 w lines ls " "$i" " title '', \\" >> "$my_var".gnuplot
    echo "  'out/"$file"' i 'process" "$i" "printing' using 3:6:7 w yerrorbars ls " "$i" " title 'node 0 -" "$i" "' axes x1y1, \\" >> "$my_var".gnuplot
#echo "  'out/"$file"' i 'data sent to" "$i""' using 3:(\$6/1000000) w lines ls " "$i" " title 'warmup ""$i""', \\" >> "$my_var".gnuplot
    done
    echo "  '' using 3:6 w lines ls 1 title '' axes x1y1, \\" >> "$my_var".gnuplot
    echo "  'out/imb.out' using 1:4 w linespoint ls 9 title 'intel',\\" >> "$my_var".gnuplot
    echo "  'out/ib_rdma_bw.out' using 1:4 w linespoint ls 10 title 'ib_write_bw'" >> "$my_var".gnuplot
    echo "unset multiplot" >> "$my_var".gnuplot
    mv "$my_var".* out
    chmod 700 out/$my_var.gnuplot
    ./out/$my_var.gnuplot

    echo "creating " "$my_var"".pdf ..."
    mv "$my_var".* out/
    echo "done"
    echo "move to out/"
done


exit 0
