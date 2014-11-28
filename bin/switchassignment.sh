#!/bin/bash

if [ $# -ne 1 ]; then
    echo "expects one parameters not" $#
    exit 0
fi

node=$1
node_temp=${node/-/}
nodenumber=${node_temp#node}
#echo $nodenumber

if [ $nodenumber -lt 2000 ];then
    if [ $nodenumber -lt 1021 ];then
        echo "switch03"
    elif [ "$nodenumber" -ge 1021 -a "$nodenumber" -lt 1045 ];then
        echo "switch02"
    else
        echo "switch01"
    fi
elif [ "$nodenumber" -ge 2000 -a "$nodenumber" -lt 3000 ];then
    if [ $nodenumber -lt 219 ];then
        echo "switch05"
    elif [ "$nodenumber" -ge 2019 -a "$nodenumber" -lt 2043 ];then
        echo "switch04"
    else
        echo "switch06"
    fi
elif [ "$nodenumber" -ge 3000 -a "$nodenumber" -lt 4000 ];then
    if [ $nodenumber -lt 3025 ];then
        echo "switch07"
    elif [ "$nodenumber" -ge 3025 -a "$nodenumber" -lt 3037 ];then
        echo "switch08"
    else
        echo "switch09"
    fi
elif [ "$nodenumber" -ge 4000 -a "$nodenumber" -lt 5013 ];then
    if [ $nodenumber -lt 4011 ];then
        echo "switch14"
    elif [ "$nodenumber" -ge 5000 ];then
        echo "switch14"

    elif [ "$nodenumber" -ge 4011 -a "$nodenumber" -lt 4035 ];then
        echo "switch13"
    elif [ "$nodenumber" -ge 4035 -a "$nodenumber" -lt 4055 ];then
        echo "switch12"
    elif [ "$nodenumber" -ge 4055 -a "$nodenumber" -lt 4079 ];then
        echo "switch11"
    else
        echo "switch10"
    fi
elif [ "$nodenumber" -ge 5013 -a "$nodenumber" -lt 6000 ];then
    if [ $nodenumber -lt 5029 ];then
        echo "switch15"
    elif [ "$nodenumber" -ge 5029 -a "$nodenumber" -lt 5053 ];then
        echo "switch16"
    elif [ "$nodenumber" -ge 5053 -a "$nodenumber" -lt 5073 ];then
        echo "switch17"
    else
        echo "switch18"
    fi
elif [ "$nodenumber" -ge 6000 -a "$nodenumber" -lt 7000 ];then
    if [ $nodenumber -lt 6021 ];then
        echo "switch21"
    elif [ "$nodenumber" -ge 6021 -a "$nodenumber" -lt 6045 ];then
        echo "switch22"
    elif [ "$nodenumber" -ge 6045 -a "$nodenumber" -lt 6070 ];then
        echo "switch20"
    else
        echo "switch19"
    fi
elif [ "$nodenumber" -ge 7000 -a "$nodenumber" -lt 8000 ];then
    if [ $nodenumber -lt 7025 ];then
        echo "switch23"
    elif [ "$nodenumber" -ge 7025 -a "$nodenumber" -lt 7049 ];then
        echo "switch24"
    elif [ "$nodenumber" -ge 7049 -a "$nodenumber" -lt 7703 ];then
        echo "switch25"
    else
        echo "switch26"
    fi

else
    echo "not in testrange"
fi

