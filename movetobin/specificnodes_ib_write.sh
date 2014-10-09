#!/bin/bash

CMD=(ib_write_bw -a -n 1000)
#CMD=(ibv_srq_pingpong)

if [ $# -ne 2 ]
then
    echo "expects two parameters not" $#
exit 0
fi

if [ "$SLURMD_NODENAME" == "$1" ]; then
    echo "testing nodes" $*
    echo "$SLURMD_NODENAME: ${CMD[*]}"
    ${CMD[*]}
elif [ "$SLURMD_NODENAME" == "$2" ]; then
    sleep 1
    echo "$SLURMD_NODENAME: ${CMD[*]} connecting to..."
    echo "$1"
    ${CMD[*]} $1
fi
