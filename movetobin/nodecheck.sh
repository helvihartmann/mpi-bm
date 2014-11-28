#!/bin/bash


size=2;
if [[ "$SLURMD_NODENAME" ]]; then
    # started by slurm



    if [[ "$2" == "srun" ]]; then
        if [[ "$SLURM_NODEID" == "$1" ]]; then
            hostname
            cat /proc/loadavg
            free
            ps
            /usr/sbin/ibstatus
        fi

    else
        for ((i=0;i<$size;i++)); do
            srun nodecheck.sh $i srun
        done
    fi
else
    #started directly
    sbatch -N$size $0
fi

exit 0
