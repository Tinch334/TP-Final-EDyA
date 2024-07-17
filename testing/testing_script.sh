#!/bin/bash

if [[ $# -ne 3 ]] || [[ $1 -lt 0 ]] || [[ $2 -lt 0 ]] || [[ $3 -lt 0 ]]
then
    echo "Usage ./testing_script.sh <height> <width> <test amount>"
    exit 1
fi

for ((i=1;i<=$3;i++)); do
    python3 ./maze_generator.py $1 $2 "maze_${i}.txt"

    ../out "maze_${i}.txt"
done