#!/bin/bash



#use *vs*.log files and sort out how big the accumulated bandwith of each phase was

for i in {0..59}
do
    grep -A 60 "phase $i" 60vs60.log | awk '{total = total + $5}END{print total}' | awk '{print " " $0}' | awk -v var2="$i" '{print var2 $0}'
done