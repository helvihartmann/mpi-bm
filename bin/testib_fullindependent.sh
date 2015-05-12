#!/bin/bash

#transforming slurms nodelist in to an array

nmbrsender=$1
nmbrnodes=$2


if [[ "$SLURMD_NODENAME" ]]; then
    readarray -t NODELIST < <(scontrol show hostname $SLURM_NODELIST)
    echo ${NODELIST[*]}
    #testing the connections between all nodes
    #by calling the script specificnodes_ib_write.sh that takes nodes to test as input parameters
    if [ "$nmbrsender" -eq "$nmbrnodes" ]; then
        for nodeid in $(seq 0 1 $((nmbrnodes-1)))
        do
            for nextnode in $(seq 0 $((nmbrnodes-1)))
            do
                if [ "$nodeid" -ne "$nextnode" ]; then
                    srun -w ${NODELIST[$nodeid]},${NODELIST[$nextnode]} ib_write_specificnodes.sh ${NODELIST[$nodeid]} ${NODELIST[$nextnode]} $nodeid $nextnode
                    #echo " "
                    #echo " "
                    #echo " "
                fi
            done
        done
    else
        echo "this is not an optional ration, duh!"
    exit 0
    fi
else
    sbatch -N$nmbrnodes $0 $nmbrsender $nmbrnodes
fi