#!/bin/bash

mkdir out
cd out
mkdir m1
mkdir m2

mv ../log/* .
mv ../55nodes*m1* m1
mv ../55nodes*m2* m2
mv ../slurm* .

mv ../nodecheck.log .

exit 0
