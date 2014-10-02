#!/bin/bash 
 
if [ "$SLURM_NODEID" == 0 ]; then 
  echo "$SLURMD_NODENAME: ib_write_bw -a -n 1000"
  ib_write_bw -a -n 1000
else
  sleep 1
  echo "$SLURMD_NODENAME: ib_write_bw -a -n 1000 node1"
  ib_write_bw -a -n 1000 node1
fi
