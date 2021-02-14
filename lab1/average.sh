#!/bin/bash
total=0
if [ $# -ne 0 ]
then 
for value in "$@"; do
	total=$(( total + value ))
done
echo "Amount of arguements: $#"
result=$(( total / $#))
echo "Average: $result"
else
echo "There are no arguements."
fi
exit 0
