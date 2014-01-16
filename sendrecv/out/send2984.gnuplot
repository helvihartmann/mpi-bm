#!/usr/bin/gnuplot 
 
 
set output 'send2984.pdf'
set terminal pdf
set logscale x 2
set xlabel "Package size [B]"
set y2label "Loadrate [%]"
set ylabel "Data rate [MB/s]"
set y2tics nomirror
set yrange [0:50]
set y2range [0:100]
set xrange [1:268435456]
set xtics ("1B" 1, " " 2, " " 4, "8B" 8," " 16, " " 32, "64B" 64, " " 128, " " 256, "512B" 512, " " 1024, " " 2048, "4kiB" 4096," " 8192, " " 16384, "32kiB" 32768, " " 65536, " " 131072, "256kiB" 262144, " " 524288, " " 1048576, "2MiB" 2097152, " " 4194304, " " 8388608, "16MiB" 16777216, " " 33554432, " " 67108864, "128MiB" 134217728," "268435456)
set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 5 ps 0.6   # --- blue
set style line 2 lc rgb '#1E90FF' lt 1 lw 1 pt 4 ps 0.2   # --- blue
plot \
  'send2984.out' using ($2):($5/1000000):($6/1000000) w yerrorbars ls 1 title 'data' axes x1y1, \
  '' using ($2):($5/1000000) w lines ls 1 title '' axes x1y1, \
  'send2984.out' using 2:($8*100) w linespoint ls 2 title '' axes x1y2
