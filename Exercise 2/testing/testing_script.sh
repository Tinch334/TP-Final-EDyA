#!/bin/bash
MAZE_DIR=mazes
if [[ $# -ne 3 ]] || [[ $1 -lt 0 ]] || [[ $2 -lt 0 ]] || [[ $3 -lt 0 ]]; then
    echo "Usage ./testing_script.sh <height> <width> <test amount>"
    exit 1
fi

if [[ ! -d "$MAZE_DIR" ]]; then
    mkdir $MAZE_DIR
fi

TOTAL_SENSOR=0
TOTAL_DISTANCE=0

for ((i = 1; i <= $3; i++)); do
    MAZE_PATH="${MAZE_DIR}/maze_${i}.txt"

    python3 ./maze_generator.py $1 $2 "$MAZE_PATH"

    RES="$(./run.sh "$MAZE_PATH" 2>&1)"
    SENSOR=$(echo "$RES" | awk -F'[^0-9]+' '{ print $2 }')
    DISTANCE=$(echo "$RES" | awk -F'[^0-9]+' '{ print $3 }')
    echo "$RES"

    TOTAL_SENSOR=$((TOTAL_SENSOR + SENSOR))
    TOTAL_DISTANCE=$((TOTAL_DISTANCE + DISTANCE))
done

echo "Average steps to solve a ${1}x${2} maze are: $((TOTAL_DISTANCE / $3))"
echo "Average sensor uses to solve a ${1}x${2} maze are: $((TOTAL_SENSOR / $3))"