#!/bin/bash
# list-glob.sh: Generating [list] in a for-loop, using "globbing" ...
# Globbing = filename expansion.
echo

for file in *.out
    do
    ls "$file" # Lists all files in $PWD (current directory).
    my_var=${file%.out}
    echo "$my_var"
    touch "$my_var".gnuplot
    echo "#!/usr/bin/gnuplot " > "$my_var".gnuplot
    echo " " >> "$my_var".gnuplot
    echo " " >> "$my_var".gnuplot
    echo "set output '"$my_var".pdf'" >> "$my_var".gnuplot
    echo "set terminal pdf" >> "$my_var".gnuplot
    echo "set logscale x 2" >> "$my_var".gnuplot
    echo 'set xlabel "Package size [B]"' >> "$my_var".gnuplot
    echo 'set y2label "total data sent [MB]"' >> "$my_var".gnuplot
    echo 'set ylabel "Data rate [MB/s]"' >> "$my_var".gnuplot
    echo "set y2tics nomirror" >> "$my_var".gnuplot
    echo "#set yrange [0:6000]" >> "$my_var".gnuplot
    echo "#set y2range [0:100]" >> "$my_var".gnuplot
    echo "set xrange [1:5e9]" >> "$my_var".gnuplot
    echo "set key right" >> "$my_var".gnuplot
    echo 'set xtics ("1B" 1, " " 2, " " 4, "8B" 8," " 16, " " 32, "64B" 64, " " 128, " " 256, "512B" 512, " " 1024, " " 2048, "4kiB" 4096," " 8192, " " 16384, "32kiB" 32768, " " 65536, " " 131072, " " 262144, "512kiB" 524288, " " 1048576, " " 2097152, " " 4194304, "8MiB" 8388608, " " 16777216, " " 33554432, " " 67108864, "128MiB" 134217728," "268435456," " 536870912," " 1073741824,"2GiB"2.147e9," "4.294e9)' >> "$my_var".gnuplot
    echo "set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 5 ps 0.6   # --- blue" >> "$my_var".gnuplot
    echo "set style line 2 lc rgb '#1E90FF' lt 1 lw 1 pt 4 ps 0.2   # --- blue" >> "$my_var".gnuplot
    echo "set style line 3 lc rgb '#FA8072' lt 1 lw 2 pt 4 ps 0.2   # --- salmon" >> "$my_var".gnuplot
    echo "set style line 4 lc rgb '#2E8B57' lt 1 lw 2 pt 4 ps 0.2   # --- green" >> "$my_var".gnuplot
    echo "plot \\" >> "$my_var".gnuplot
    echo "  'out/"$file"' using 3:6:7 w yerrorbars ls 1 title 'data' axes x1y1, \\" >> "$my_var".gnuplot
    echo "  '' using 3:6 w lines ls 1 title '' axes x1y1, \\" >> "$my_var".gnuplot
    echo "  'out/"$file"' using 3:(\$1/1000000) w linespoint ls 2 title 'total data' axes x1y2,\\" >> "$my_var".gnuplot
    echo "  'out/imb.out' using 1:4 w linespoint ls 3 title 'intel',\\" >> "$my_var".gnuplot
    echo "  'out/ib_rdma_bw.out' using 1:4 w linespoint ls 4 title 'ib_write_bw'" >> "$my_var".gnuplot
    mv "$my_var".* out
    chmod 700 out/$my_var.gnuplot
    ./out/$my_var.gnuplot
    mv "$my_var".* out
done


exit 0
