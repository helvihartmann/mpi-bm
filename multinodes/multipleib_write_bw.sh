#!/bin/bash


# start with srun -N2 -l yourib_write_bw.sh 1 2

nmbrsender=$1
nmbrnodes=$2
name="$1vs$1.out"
echo "number of senders: $nmbrsender, number of nodes: $nmbrnodes"
if [ $# -ne 2 ]; then
    echo "expects two parameters not" $#
exit 0
fi

if [[ "$SLURMD_NODENAME" ]]; then
    # started by slurm
    readarray -t NODES < <(scontrol show hostname $SLURM_NODELIST)

    for ((size=4;size<=512*1024;size*=2)); do
        iters=10000
        if ((size>=4096)); then
            ((iters=2000000000/size))
        fi

        CMD=(ib_write_bw -s "$size" -N -n "$iters" -p)

        if [ "$SLURM_NODEID" -lt "$nmbrsender" ]; then
            #server opening ports
            for ((recvid=nmbrsender;recvid<nmbrnodes;recvid++));do
                ${CMD[*]} "$((18000+recvid))"  > /dev/null &
            done

        else
            #clients connecting to ports
            for ((sendid=0;sendid<nmbrsender;sendid++));do
                sleep 1
                ${CMD[*]} "$((18000+SLURM_NODEID))" ${NODES["$sendid"]} | grep -A 1 "BW average" | tail -1 | awk -v var2="${NODES[$sendid]}" '{print var2 $0}' &
            done
        fi
        wait
    done

    #resorts output and writes it into an <name>.ib file
    if [ "$SLURM_NODEID" == 0 ]; then
        for file in $name
        do
            my_var=${file%.out}
            echo "creating " "$my_var"".ib ..."
            touch "$my_var".ib
            for ((recvid=s;recvid<nmbrnodes;recvid++));do
                for ((sendid=0;sendid<nmbrsender;sendid++));do
                    grep "$recvid: ${NODES[$sendid]}" $file >> "$my_var".ib
                    echo " " >> "$my_var".ib
                    echo " " >> "$my_var".ib
                done
            done

        done
    fi

else
    #started directly
    srun -N$nmbrnodes -l $0 $1 $2 | tee $name
fi

exit 0