#!/bin/bash
# list-glob.sh: Generating [list] in a for-loop, using "globbing" ...
# Globbing = filename expansion.
echo

for file in *.out
    do
    ls "$file" # Lists all files in $PWD (current directory).
    hase = $3
    echo $hase
    numberprocesses = $(awk '/processes/ {print $3}' $file)
    echo $numberprocesses
   done

exit 0
