#!/bin/bash

for p in {0..59}
do
    for s in {0..59}
    do
        d=$(((((s % 5)+(p % 5)) % 5)*12+((s/5)+(p/5)) % 12))
       echo "$s to $d"
    done
    echo " "
done

