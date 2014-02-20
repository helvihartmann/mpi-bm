#!/bin/bash
# list-glob.sh: Generating [list] in a for-loop, using "globbing" ...
# Globbing = filename expansion.
echo
START=1
for file in *.out
do
    ls "$file" # Lists all files in $PWD (current directory).
    hase=$"hase"
    echo $hase
    END=$(awk '/processes/ {print $3;exit}' $file)
    echo $END

    for i in $(eval echo "{$START..$END}")
    do
        echo "$i "
    done
done

exit 0
0
