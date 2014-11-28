#!/bin/bash

CMD=(ib_write_bw -a -n 1000)
#CMD=(ibv_srq_pingpong)

if [[ "$SLURMD_NODENAME" ]]; then
    # started by slurm
    if [ "$SLURM_NODEID" == 0 ]; then
        echo "$SLURMD_NODENAME: ${CMD[*]}"
        ${CMD[*]}
    else
        sleep 1
        echo "$SLURMD_NODENAME: ${CMD[*]} connecting to..."
        remotehost=$(scontrol show hostname $SLURM_NODELIST | grep -v $SLURMD_NODENAME)
        echo $remotehost
        ${CMD[*]} $remotehost
    fi
else
    # started directly
    srun -N2 -l "$0"
fi