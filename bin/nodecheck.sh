#!/bin/bash



if [[ "$SLURMD_NODENAME" ]]; then
    # started by slurm

    size=$SLURM_NNODES
    readarray -t NODES < <(scontrol show hostname $SLURM_NODELIST)

    if [[ "$2" == "srun" ]]; then
        if [[ "$SLURM_NODEID" == "$1" ]]; then
            hostname
            echo "cat /proc/loadavg"
            cat /proc/loadavg | awk '{print ": " $0}'| awk -v var1="${NODES[$SLURM_NODEID]}" '{print var1 $0}'
            echo " "
            echo "free"
            free | awk '{print ": " $0}'| awk -v var1="${NODES[$SLURM_NODEID]}" '{print var1 $0}'
            echo " "
            echo "ps auxww"
            ps auxww | awk '{print ": " $0}'| awk -v var1="${NODES[$SLURM_NODEID]}" '{print var1 $0}'
            echo " "
            echo "ibstat"
            ibstat | awk '{print ": " $0}'| awk -v var1="${NODES[$SLURM_NODEID]}" '{print var1 $0}'
            echo " "
            echo "mpstat"
            mpstat | awk '{print ": " $0}'| awk -v var1="${NODES[$SLURM_NODEID]}" '{print var1 $0}'
            echo "##################################################"
            echo " "
            echo " "
        fi

    else
        for ((i=0;i<$size;i++)); do

            echo ${NODES[*]}
            srun nodecheck.sh $i srun
        done
    fi
else
    #started directly
    size=2;
    sbatch -N$size $0
fi

exit 0
