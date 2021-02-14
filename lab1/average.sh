#!/bin/bash
total=0
if [ $# -ne 0 ]
then 
for value in "$@"; do
	total=$(( total + value ))
done
echo "Amount of arguments: $#"
result=$(( total / $#))
echo "Average: $result"
else
echo "There are no arguments."
fi
exit 0
