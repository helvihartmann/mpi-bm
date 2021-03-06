#!/bin/bash


name="$1vs$1.log"
if [ $# -lt 1 ]; then
    echo "expects one parameters not" $#
    exit 0
fi
nmbrnodes=$1



#####################################################################################################

if [[ "$SLURMD_NODENAME" ]]; then
    # started by slurm-------------------------------------------------------
    readarray -t NODES < <(scontrol show hostname $SLURM_NODELIST)
    NODES_DOUBLE_tmp=( "${NODES[@]}" "${NODES[@]}")
    readarray -t NODES_DOUBLE < <(for a in "${NODES_DOUBLE_tmp[@]}"; do echo "$a"; done | sort)

    #Second Part: performs bandwidth test-----------------------------------------------------------------
    if [[ "$3" == "srun" ]]; then
        size=$2
        nextnode_tmp=$4
        nextnode=$((nextnode_tmp+2))
        iters=200000 #1000000
        if ((size>=4096)); then
            ((iters=1000000000/size))
        fi

        CMD=(ib_write_bw -s "$size" -n 1000)
#CMD=(ib_send_lat)


        if [ $((SLURM_PROCID%2)) -eq 0 ]; then
           ${CMD[*]} > /dev/null
        else
            node=${NODES_DOUBLE["$SLURM_PROCID"]}
            sendernode=${NODES_DOUBLE[$((SLURM_PROCID-nextnode))]}
            sleep 1
            ${CMD[*]} $sendernode | grep -A 1 "bytes" | tail -1 | awk -v var1="$node " '{print var1 $0}' | awk -v var2="$sendernode" '{print var2 $0}'
        fi



    #First part: starts test of certain package size----------------------------------------------------------------------------
    else
        echo ${NODES[*]}

        nmbrproc_all=${#NODES[@]}

        # there are alsp passive processes on nodes which are not used for benchmark tests.
        #so one or two processes have to be started on every node but only the processes on tested nodes are called active
        for ((size=524288;size<=512*1024;size*=2)); do #512*1024
            i=0
            for ((nextnode=0;nextnode<(2*nmbrproc_all);nextnode=nextnode+2)); do
                echo "phase $i"
                srun --ntasks-per-node 2 -n $((2*nmbrproc_all)) testib_fast.sh $nmbrproc_all $size srun $nextnode
                i=$((i+1))
                echo " "
                echo " "
            done
        done

        my_var="$1vs$1.hist"
        echo "creating $my_var. ... $m"
        echo ${NODES[*]} > "$my_var"
        if [ "$SLURM_PROCID" -eq "0" ]; then
            for size in 524288;do #512*1024
                for ((recvid=0;recvid<nmbrproc_all;recvid++));do
                    for ((sendid=0;sendid<nmbrproc_all;sendid++));do
                        receiver=${NODES[$recvid]}
                        sender=${NODES[$sendid]}
#echo "$receiver$sender $size" >> "$my_var"
grep "$receiver$sender \+$size" $name | awk -v var1="$sendid " '{print var1 $0}' | awk '{print " " $0}' | awk -v var2="$recvid" '{print var2 $0}' >> "$my_var"
                    done
                    #echo " " >> "$my_var"
                done
                echo " " >> "$my_var"
                echo " " >> "$my_var"
            done
        fi

    fi




else
    #started directly--------------------------------------------------------------------------------------------------------------
    sbatch -o $name -N$nmbrnodes $0 $1
fi

exit 0