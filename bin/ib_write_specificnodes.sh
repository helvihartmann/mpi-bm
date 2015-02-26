#!/bin/bash

CMD=(ib_write_bw -a -n 1000)
#CMD=(ib_write_bw -a -d mlx5_0 -n 1000)
#CMD=(ibv_srq_pingpong)

if [ $# -ne 2 ]
then
    echo "expects two parameters not" $#
exit 0
    fi

if [[ "$SLURMD_NODENAME" ]]; then
# started by slurm
    if [ "$SLURMD_NODENAME" == "$1" ]; then
        ${CMD[*]} > /dev/null
    elif [ "$SLURMD_NODENAME" == "$2" ]; then
        sleep 1
        ${CMD[*]} $1
    fi
else
    # started directly
    srun -w ${NODES[0]},${NODES[1]} -N2 $0 $1 $2
fi
