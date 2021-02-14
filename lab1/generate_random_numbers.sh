#!/bin/bash
count=150
while [ $count -ne 1 ]; do
echo -n "$(od -An -N2 -i /dev/random)"
count=$(( count - 1 ))
done
echo "$(od -An -N2 -i /dev/random)"
exit 0 
