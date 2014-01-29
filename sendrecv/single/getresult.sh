#!/bin/bash

echo
touch single.out
for file in *.out
	do
	sed -n '$p' $file >> single.out
done
