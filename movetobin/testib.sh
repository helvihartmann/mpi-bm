#!/bin/bash

#transforming slurms nodelist in to an array
readarray -t NODELIST < <(scontrol show hostname $SLURM_NODELIST)
echo ${NODELIST[*]}

#testing the connections between all nodes: 0-2, 3-4 ...(n-1)-n/0
#by calling the script specificnodes_ib_write.sh that takes nodes to test as input parameters
s=$1
n=$2
for nodeid in $(seq 0 1 $((s-1)))
do
    for nextnode in $(seq $1 $((n-1)))
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