#!/bin/bash

#transforming slurms nodelist in to an array
string=$(scontrol show hostname $SLURM_NODELIST | paste -d, -s)
text="beginning"
i=1
while [ "$text" != "" ]
do
    text=`echo $string | cut -d , -f $i`
    i=$((i+1))
    NODELIST+=("$text")
done
echo ${NODELIST[*]}

#testing the connections between all nodes: 0-2, 3-4 ...(n-1)-n/0
#by calling the script specificnodes_ib_write.sh that takes nodes to test as input parameters
for nodeid in $(seq 0 2 $((SLURM_NNODES-1)))
do
    nextnode=$(expr $((nodeid+1)) % $SLURM_NNODES)
    echo "nodeid: $nodeid, nextnode: $nextnode, number nodes: $SLURM_NNODES"
    srun -w ${NODELIST[$nodeid]},${NODELIST[$nextnode]} -l specificnodes_ib_write.sh ${NODELIST[$nodeid]} ${NODELIST[$nextnode]}
done