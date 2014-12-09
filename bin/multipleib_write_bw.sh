#!/bin/bash

serverconnect() {
    #server opening ports
    #nodes should actually not connect to itself, but if clause here destroyes processes occuring simultanesouly. hence they do not connect but are filtered out later when writing the ib file
    start=$1
    for ((recvid=start;recvid<nmbrproc_active;recvid=recvid+m));do
        ${CMD[*]} "$((18000+recvid))"  > /dev/null &
    done
}
clientconnect() {
    #clients connecting to ports
    for ((sendid=0;sendid<nmbrsender;sendid++));do
        sleep 1
        ${CMD[*]} "$((18000+SLURM_PROCID))" ${NODES["$sendid"]} | grep -A 1 "BW average" | tail -1 | awk -v var1="${NODES[SLURM_NODEID]}" '{print var1 $0}' | awk -v var2="${NODES[$sendid]}" '{print var2 $0}' &
    done
}
#####################################################################################################
name="$1vs$1.log"
if [ $# -lt 2 ]; then
echo "expects two parameters not" $#
exit 0
fi
nmbrsender=$1
nmbrnodes=$2

if [ "$nmbrsender" -eq "$((nmbrnodes/2))" ]; then
    m=1
elif [ "$nmbrsender" -eq "$nmbrnodes" ]; then
    m=2
else
    echo "this is not an optional ration, duh!"
    exit 0
fi

nmbrproc_active=$((m*nmbrnodes))
#####################################################################################################

if [[ "$SLURMD_NODENAME" ]]; then
    # started by slurm-------------------------------------------------------
    readarray -t NODES < <(scontrol show hostname $SLURM_NODELIST)

    #Second Part----------------------------------------------------------------------------------------------------
    if [[ "$4" == "srun" ]]; then
        size=$3
        iters=200000 #1000000
        if ((size>=4096)); then
            ((iters=1000000000/size))
        fi

        CMD=(ib_write_bw -s "$size" -N -n "$iters" -p)

        #for one process per node m=1
        if [ "$m" -eq "1" ]; then
            if [ "$SLURM_PROCID" -lt "$nmbrsender" ]; then
                serverconnect $nmbrsender
            elif [ "$SLURM_PROCID" -lt "$nmbrproc_active" ]; then
                clientconnect
            fi
            wait

        #for two processes per node m=2. sender are even and receiver odd numbered
        elif [ "$m" -eq "2" ]; then
            if [ "$SLURM_PROCID" -lt "$nmbrproc_active" ]; then

                if [ $((SLURM_PROCID%2)) -eq 0 ]; then
                   serverconnect 1
                else
                    clientconnect
                fi
                wait
            fi

        else
            echo "failure m must be 1 or 2 not $m"
        fi

    #First part--------------------------------------------------------------------------------------------------------------
    else
        echo ${NODES[*]}
        length=${#NODES[@]}
        nmbrproc_all=$((m*length))
        # there are alsp passive processes on nodes which are not used for benchmark tests. so one or two processes have to be started on every node but only the processes on tested nodes are called active
        for ((size=4;size<=512*1024;size*=2)); do
            srun -l -n $nmbrproc_all multipleib_write_bw.sh $nmbrsender $nmbrnodes $size srun
        done

        #resorts output and writes it into an <name>.ib file-------------------------------------------------------
        for file in *.log #$name
        do
            my_var=${file%.log}
            echo "creating $my_var.ib ... $m"
            touch "$my_var".ib
            echo ${NODES[*]} >> "$my_var".ib
            for ((recvid=nmbrsender;recvid<nmbrproc_active;recvid++));do
                for ((sendid=0;sendid<nmbrsender;sendid++));do
                    if [ "$m" -eq "2" ]; then
                        recvid_tmp=$((recvid-nmbrsender))
                        if [ "$recvid_tmp" -eq "$sendid" ]; then
                            break;
                        fi

                    else
                        recvid_tmp=recvid
                    fi
                    sendswitch=$(switchassignment.sh ${NODES[$sendid]})
                    recvswitch=$(switchassignment.sh ${NODES[$recvid_tmp]})
                    echo "# $sendswitch$recvswitch ---${NODES[$sendid]}${NODES[$recvid_tmp]}" >> "$my_var".ib
                    grep "${NODES[$sendid]}${NODES[$recvid_tmp]}" $file >> "$my_var".ib
                    echo " " >> "$my_var".ib
                    echo " " >> "$my_var".ib
                done
            done
            mkdir -p log
            mv *.log log
        done
    fi

else
    #started directly--------------------------------------------------------------------------------------------------------------
    sbatch -o $name -N$nmbrnodes $0 $1 $2
fi

exit 0