#!/usr/local/bin/gnuplot

set output 'sendrecv.pdf'
set terminal pdf
set logscale x 2
set xlabel "Package size [B]"
set xlabel "Package size [B]"
set yrange [1:4]
#set xrange [512:8192]
set title "Data rate MPI send receive \n I,II sent at the same time; III sent separately"
#set key at 4048576,3000
set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 5 ps 1.2   # --- blue
set style line 2 lc rgb '#AFEEEE' lt 1 lw 2 pt 5 ps 1   # --- lightblue
set style line 3 lc rgb '#2E8B57' lt 1 lw 2 pt 5 ps 1   # --- green
set style line 4 lc rgb '#98FB98' lt 1 lw 2 pt 5 ps 1   # --- lightgree
set style line 5 lc rgb '#FF0000' lt 1 lw 2 pt 5 ps 1   # --- red
set style line 6 lc rgb '#FA8072' lt 1 lw 2 pt 5 ps 1   # --- salmon
set style line 7 lc rgb '#8A2BE2' lt 1 lw 2 pt 5 ps 1   # --- violet
set style line 8 lc rgb '#9370DB' lt 1 lw 2 pt 5 ps 1   # --- purple
set style line 9 lc rgb '#8B4513' lt 1 lw 2 pt 5 ps 1   # --- brown
set style line 10 lc rgb '#CD853F' lt 1 lw 2 pt 5 ps 1   # --- mocca
plot \
    'send2.out' using 2:($4/1000000):($5/1000000) w yerrorbars ls 1 title 'send', \
    ''using 2:($3/1000000) w lines ls 1 title '',\
