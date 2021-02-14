#!/bin/bash
count=150
while [ $count -ne 0 ]; do
od -An -N2 -i /dev/random
count=$(( count - 1 ))
done
exit 0 
