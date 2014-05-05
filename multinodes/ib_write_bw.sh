#!/bin/bash

NODES=(node0 node1)
#CMD=(ib_write_bw -a -n 10)
#CMD=(ib_write_bw -s 8388608 -n 1000)
#CMD=(ib_write_bw -a -n 1000 -I 10)
CMD=(ib_write_bw -a -n 1000)
#CMD=(ib_write_lat -a -n 1000)
#CMD=(ib_read_bw -a -n 5000)

if [[ "$SLURMD_NODENAME" ]]; then
        # started by slurm
	if [ "$SLURM_NODEID" == 0 ]; then
		echo "$SLURMD_NODENAME: ${CMD[*]}"
		${CMD[*]}
	else
		sleep 1
		echo "$SLURMD_NODENAME: ${CMD[*]} ${NODES[0]}"
		${CMD[*]} ${NODES[0]}
	fi
else
        # started directly
        srun -w ${NODES[0]},${NODES[1]} -N2 -l "$0"
fi
