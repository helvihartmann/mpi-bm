#!/bin/bash
echo "  ,.,  "
echo "  (o,o)"
echo " <    >"
echo "  _'""'__"
echo " "
rm scatter*.out
sbatch scatter.in 
#echo sbatch pingpong.in
sleep 2
echo starting script
tail -f scatter*.out
exit 0
