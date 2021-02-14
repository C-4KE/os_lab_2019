#!/bin/bash
count=150
temp=""
while [ $count -ne 0 ]; do
temp="$temp$(od -An -N2 -i /dev/random)"
count=$(( count - 1 ))
done
echo "$(echo "$temp" | tr -s " ")"
exit 0 
