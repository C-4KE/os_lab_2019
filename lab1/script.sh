#!/bin/bash
echo "Current path: "
pwd
dt = $(date '+%d/%m/%Y %H:%M:%S')
echo "Current date and time: $dt"
echo "Data of the PATH variable: "
printenv PATH
exit 0
