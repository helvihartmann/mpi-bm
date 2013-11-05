#!/bin/bash
# list-glob.sh: Generating [list] in a for-loop, using "globbing" ...
# Globbing = filename expansion.

echo

for file in slurm*.cpp
#           ^  Bash performs filename expansion
#+             on expressions that globbing recognizes.
do
ls "$file"      # Lists all files in $PWD (current directory).
sbatch "$file"
done

echo; echo

exit 0
