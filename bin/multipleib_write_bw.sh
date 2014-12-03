#!/bin/bash

# start with srun -N2 -l yourib_write_bw.sh 1 2

name="$1vs$1.log"
#echo "number of senders: $nmbrsender, number of nodes: $nmbrnodes"
if [ $# -lt 2 ]; then
echo "expects two parameters not" $#
exit 0
fi

nmbrsender=$1
nmbrnodes=$2

if [ "$nmbrsender" -eq "$((nmbrnodes/2))" ]; then
    m=1
#donotconnectto=$((SLURM_PROCID+nmbrproc))
elif [ "$nmbrsender" -eq "$nmbrnodes" ]; then
    m=2
#donotconnectto=$(((SLURM_PROCID+nmbrsender)%nmbrproc))
else
    echo "this is not an optional ration, duh!"
    exit 0
fi

nmbrproc=$((m*nmbrnodes))

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
# if [ "$recvid" -ne "$donotconnectto" ]; then
                    ${CMD[*]} "$((18000+recvid))"  > /dev/null &
#fi
            done

            else
            #clients connecting to ports
            for ((sendid=0;sendid<nmbrsender;sendid++));do
#if [ "$sendid" -ne "$donotconnectto" ]; then
                    sleep 1
                    ${CMD[*]} "$((18000+SLURM_PROCID))" ${NODES["$sendid"]} | grep -A 1 "BW average" | tail -1 | awk -v var1="${NODES[SLURM_NODEID]}" '{print var1 $0}' | awk -v var2="${NODES[$sendid]}" '{print var2 $0}' &
#fi
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
            echo "creating $my_var.ib ... $m"
            touch "$my_var".ib
            echo ${NODES[*]} >> "$my_var".ib
            for ((recvid=nmbrsender;recvid<nmbrproc;recvid++));do
                for ((sendid=0;sendid<nmbrsender;sendid++));do
                    echo $recvid $sendid $m
                    if [ "$m" -eq "2" ]; then
                        recvid_tmp=$((recvid-nmbrsender))
                        echo $recvid
                        if [ "$recvid_tmp" -eq "$sendid" ]; then
                            break;
                        fi

                    else
                        recvid_tmp=recvid
                    fi
                    sendswitch=$(switchassignment.sh ${NODES[$sendid]})
                    recvswitch=$(switchassignment.sh ${NODES[$recvid_tmp]})
#echo "# --${NODES[$sendid]}${NODES[$recvid_tmp]}" >> "$my_var".ib
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
    #started directly
    sbatch -o $name -N$nmbrnodes $0 $1 $2
fi

exit 0
