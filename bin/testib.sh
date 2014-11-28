#!/bin/bash

#transforming slurms nodelist in to an array


nmbrsender=$1
nmbrnodes=$2


if [[ "$SLURMD_NODENAME" ]]; then
    readarray -t NODELIST < <(scontrol show hostname $SLURM_NODELIST)
    echo ${NODELIST[*]}
    #testing the connections between all sender (first n nodes) and receivers (last n nodes)
    #by calling the script specificnodes_ib_write.sh that takes nodes to test as input parameters
    if [ "$nmbrsender" -eq "$((nmbrnodes/2))" ]; then
        for nodeid in $(seq 0 1 $((nmbrsender-1)))
        do
            for nextnode in $(seq $1 $((nmbrnodes-1)))
            do
                #nextnode=$(expr $((nodeid+1)) % $SLURM_NNODES)
                echo "nodeid: $nodeid, nextnode: $nextnode, number nodes: $SLURM_NNODES"
                echo "#testing nodes" ${NODELIST[$nodeid]} ${NODELIST[$nextnode]}
                srun -w ${NODELIST[$nodeid]},${NODELIST[$nextnode]} -l specificnodes_ib_write.sh ${NODELIST[$nodeid]} ${NODELIST[$nextnode]}
                echo " "
                echo " "
                echo " "
            done
        done
    #testing the connections between all nodes
    #by calling the script specificnodes_ib_write.sh that takes nodes to test as input parameters
    elif [ "$nmbrsender" -eq "$nmbrnodes" ]; then
        for nodeid in $(seq 0 1 $((nmbrnodes-1)))
        do
            for nextnode in $(seq 0 $((nmbrnodes-1)))
            do
                if [ "$nodeid" -ne "$nextnode" ]; then
                    #nextnode=$(expr $((nodeid+1)) % $SLURM_NNODES)
                    echo "nodeid: $nodeid, nextnode: $nextnode, number nodes: $SLURM_NNODES"
                    echo "#testing nodes" ${NODELIST[$nodeid]} ${NODELIST[$nextnode]}
                    srun -w ${NODELIST[$nodeid]},${NODELIST[$nextnode]} -l specificnodes_ib_write.sh ${NODELIST[$nodeid]} ${NODELIST[$nextnode]}
                    echo " "
                    echo " "
                    echo " "
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