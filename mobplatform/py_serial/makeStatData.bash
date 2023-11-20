#!/bin/bash

STATDATA="statData.txt"
SCRIPT="calibration.py"

cp /dev/null $STATDATA
numOfCycles=10
n=0
while [ $n -le $numOfCycles ]
do
 echo "current bash cycle is ${n}"
 python  $SCRIPT
 sleep 1 
 n=$((n+1))
done

echo "Bash script finished"
