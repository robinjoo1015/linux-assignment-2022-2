#!/bin/sh
for i in $(seq 1 $1)
do
	line=""
	for j in $(seq 1 $2)
	do
		mul=`expr $i \* $j`
		line+="$i*$j=$mul\t"
	done
	echo -e $line
done 
