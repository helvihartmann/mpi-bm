#!/bin/bash

# start with sbatch -o 60vs60.log --nodefile=nodelist.txt opttestib_fast.sh 60 works only for 60 nodes spread eually on 5 switches (12 each)
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
    readarray -t NODENAMES_DOUBLE < <(for a in "${NODES_DOUBLE_tmp[@]}"; do echo "$a"; done | sort)

    NODEIDS_DOUBLE_tmp=(`seq 0 1 59` `seq 0 1 59`)
    readarray -t NODEIDS_DOUBLE < <(for a in "${NODEIDS_DOUBLE_tmp[@]}"; do echo "$a"; done | sort)
#echo "${NODEIDS_DOUBLE[*]}"

    #Second Part: performs bandwidth test-----------------------------------------------------------------
    if [[ "$3" == "srun" ]]; then
        size=$2
        phase=$4
        iters=200000 #1000000
        if ((size>=4096)); then
            ((iters=1000000000/size))
        fi

        CMD=(ib_write_bw -s "$size" -n 1000)
        #CMD=(ib_send_lat)


        if [ $((SLURM_PROCID%2)) -eq 0 ]; then
        #echo " "
        ${CMD[*]} > /dev/null
        else
            sleep 1
            node=${NODENAMES_DOUBLE["$SLURM_PROCID"]}
            nodeid=${NODEIDS_DOUBLE["$SLURM_PROCID"]}
            sendernodeid=$(((((nodeid % 5)+(phase % 5)) % 5)*12+((nodeid/5)+(phase/5)) % 12))
            sendernode=${NODES["$sendernodeid"]}
            #echo "$node $nodeid from $sendernodeid $sendernode"

            ${CMD[*]} $sendernode | grep -A 1 "bytes" | tail -1 | awk -v var1="$node " '{print var1 $0}' | awk -v var2="$sendernode" '{print var2 $0}'
        fi



    #First part: starts test of certain package size----------------------------------------------------------------------------
    else
        echo ${NODES[*]}

        nmbrproc_all=${#NODES[@]}

        # there are alsp passive processes on nodes which are not used for benchmark tests.
        #so one or two processes have to be started on every node but only the processes on tested nodes are called active
        for ((size=524288;size<=512*1024;size*=2)); do #512*1024
            for ((phase=0;phase<nmbrproc_all;phase++)); do
                echo "phase $phase"
                srun --ntasks-per-node 2 -n $((2*nmbrproc_all)) opttestib_fast.sh $nmbrproc_all $size srun $phase
                i=$((i+1))
                echo " "
                echo " "
            done
        done

        # sorting the loq file to a hist file
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