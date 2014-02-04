#!/bin/bash
rm *.out
sbatch pingpongSsend.in
#echo sbatch pingpong.in
sleep 2
echo starting script
tail -f *.out
exit 0
