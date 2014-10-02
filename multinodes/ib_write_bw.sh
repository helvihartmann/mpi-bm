#!/bin/bash

NODES=($*)


echo "nodes" $*

if [ $# -ne 2 ]
then
    echo "expects two parameters not" $#
    exit 0
fi

#CMD=(ib_write_bw -a -n 10)
#CMD=(ib_write_bw -s 8388608 -n 1000)
#CMD=(ib_write_bw -a -n 1000 -I 10)
CMD=(ib_write_bw -a -n 1000)
#CMD=(ib_write_lat -a -n 1000)
#CMD=(ib_read_bw -a -n 5000)


echo "#!/bin/bash " > slurmib_write_bw.sh
echo " " >> slurmib_write_bw.sh
echo 'if [ "$SLURM_NODEID" == 0 ]; then ' >> slurmib_write_bw.sh
echo "  echo '"'$SLURMD_NODENAME: '${CMD[*]}''"'" >> slurmib_write_bw.sh
echo "  ${CMD[*]}" >> slurmib_write_bw.sh
echo "else" >> slurmib_write_bw.sh
echo "  sleep 1" >> slurmib_write_bw.sh
echo '  echo "$SLURMD_NODENAME: ${CMD[*]} ${NODES[0]}"' >> slurmib_write_bw.sh
echo "  ${CMD[*]} ${NODES[0]}" >> slurmib_write_bw.sh
echo "fi" >> slurmib_write_bw.sh

chmod 755 slurmib_write_bw.sh

# started directly
srun -w ${NODES[0]},${NODES[1]} -N2 -l slurmib_write_bw.sh
