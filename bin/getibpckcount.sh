#!/bin/bash

#script runs only on outputs of nodecheck.sh started within the same batch script
if [[ "$SLURMD_NODENAME" ]]; then
    # started by slurm-------------------------------------------------------
    readarray -t NODES < <(scontrol show hostname $SLURM_NODELIST)
    nmbrnodes=${#NODES[@]}
    
    for nodes in ${NODES[*]}
    do
        for device in mlx4 mlx5
        do
            for port in 1 2
            do
                startcount=$(grep -A 1 "$nodes $device $port" prenodecheck.log | tail -1 | awk '{print $2}')
                endcount=$(grep -A 1 "$nodes $device $port" postnodecheck.log | tail -1 | awk '{print $2}')
                result=$((endcount-startcount))
                echo "$nodes $device $port = $endcount - $startcount = $result"


            done
        done
    done
else
    echo "dont use it like this, put it in the same batch script with nodecheck.sh"
fi

exit 0
