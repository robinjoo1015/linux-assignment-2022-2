#!/bin/sh
if [ $# -eq 2 ]
then
	if [ $1 -le 0 ] || [ $2 -le 0 ]
	then
		echo "Input must be greater than 0"
		exit 0
	fi
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
else
	echo "Invalid input"
	exit 0
fi
