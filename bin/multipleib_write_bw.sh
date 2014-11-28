#!/bin/bash

# start with srun -N2 -l yourib_write_bw.sh 1 2

nmbrsender=$1
nmbrnodes=$2

if [ "$nmbrsender" -eq "$((nmbrnodes/2))" ]; then
    nmbrproc=$nmbrnodes
elif [ "$nmbrsender" -eq "$nmbrnodes" ]; then
    nmbrproc=$((2*nmbrnodes))
else
    echo "this is not an optional ration, duh!"
    exit 0
fi

name="$1vs$1.log"
#echo "number of senders: $nmbrsender, number of nodes: $nmbrnodes"
if [ $# -lt 2 ]; then
    echo "expects two parameters not" $#
exit 0
fi

if [[ "$SLURMD_NODENAME" ]]; then
    # started by slurm
    readarray -t NODES < <(scontrol show hostname $SLURM_NODELIST)

    if [[ "$4" == "srun" ]]; then

#hostname
        size=$3
        iters=100000 #1000000
        if ((size>=4096)); then
            ((iters=1000000000/size))
        fi

        CMD=(ib_write_bw -s "$size" -N -n "$iters" -p)

        if [ "$SLURM_PROCID" -lt "$nmbrsender" ]; then
            #server opening ports
            for ((recvid=nmbrsender;recvid<nmbrproc;recvid++));do
                ${CMD[*]} "$((18000+recvid))"  > /dev/null &
            done

            else
            #clients connecting to ports
            for ((sendid=0;sendid<nmbrsender;sendid++));do
                sleep 1
                ${CMD[*]} "$((18000+SLURM_PROCID))" ${NODES["$sendid"]} | grep -A 1 "BW average" | tail -1 | awk -v var1="${NODES[SLURM_NODEID]}" '{print var1 $0}' | awk -v var2="${NODES[$sendid]}" '{print var2 $0}' &
            done
        fi
        wait
    else


        echo ${NODES[*]}

        for ((size=4;size<=512*1024;size*=2)); do
            srun --distribution=cyclic -l -n $nmbrproc multipleib_write_bw.sh $nmbrsender $nmbrnodes $size srun
        done

        #resorts output and writes it into an <name>.ib file
        for file in *.log #$name
        do
            my_var=${file%.log}
            echo "creating $my_var.ib ..."
            touch "$my_var".ib
            echo ${NODES[*]} >> "$my_var".ib
            for ((recvid=nmbrsender;recvid<nmbrnodes;recvid++));do
                for ((sendid=0;sendid<nmbrsender;sendid++));do
                    sendswitch=$(switchassignment.sh ${NODES[$sendid]})
                    recvswitch=$(switchassignment.sh ${NODES[$recvid]})
                    echo "# $sendswitch$recvswitch ---${NODES[$sendid]}${NODES[$recvid]}" >> "$my_var".ib
                    grep "${NODES[$sendid]}${NODES[$recvid]}" $file >> "$my_var".ib
                    echo " " >> "$my_var".ib
                    echo " " >> "$my_var".ib
                done
            done

        done
        mkdir log
        mv *.log log
    fi

else
    #started directly
    sbatch -o $name -N$nmbrnodes $0 $1 $2
fi

exit 0
