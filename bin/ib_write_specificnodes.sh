#!/bin/bash

CMD=(ib_write_bw -s 524288 -n 1000)
#CMD=(ib_write_bw -a -n 1000)
#CMD=(ibv_srq_pingpong)

if [ $# -lt 2 ]
then
    echo "expects at least two parameters not" $#
exit 0
    fi

if [[ "$SLURMD_NODENAME" ]]; then
# started by slurm

    if [ "$SLURMD_NODENAME" == "$1" ]; then
        ${CMD[*]} > /dev/null
    elif [ "$SLURMD_NODENAME" == "$2" ]; then
        sleep 1
        #${CMD[*]} $1
        ${CMD[*]} $1 | grep -A 1 "bytes" | tail -1 | awk -v var4="$2 " '{print var4 $0}' | awk -v var3="$1" '{print var3 $0}' | awk -v var2="$4 " '{print var2 $0}' | awk '{print " " $0}' | awk -v var1="$3" '{print var1 $0}'
    fi
else
    # started directly
    srun -w ${NODES[0]},${NODES[1]} -N2 $0 $1 $2
fi
