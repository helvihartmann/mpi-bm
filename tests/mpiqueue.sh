#!/bin/bash
echo "  ,.,  "
echo "  (o,o)"
echo " <    >"
echo "  _'""'__"
echo " "
rm *.out
sbatch mpiqueue.in 
#echo sbatch pingpong.in
sleep 2
echo starting script
tail -f *.out
exit 0
