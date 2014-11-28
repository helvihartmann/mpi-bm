#!/bin/bash

if [ $# -ne 1 ]; then
    echo "expects one parameters not" $#
    exit 0
fi

node=$1

switch=$(switchassignment.sh $node)

echo "getswitch reports:" $switch