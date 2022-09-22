#!/bin/sh
re="^[1-9][0-9]*$"
if [ $# -eq 2 ]
then
	if ! [[ $1 =~ $re ]] || ! [[ $2 =~ $re ]]
	then
		echo "Invalid input number, input must be greater than 0"
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
	echo "Invalid parameter numbers"
	exit 0
fi
