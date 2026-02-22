#!/bin/bash

# 所有子任务的模式数量
declare -A CNT
CNT[1]=10
CNT[2]=5
CNT[3]=10
CNT[4]=10
CNT[5]=10
CNT[6]=10
CNT[7]=10
rm -r data/
mkdir -p data
for X in {1..7}; do
    YMAX=${CNT[$X]}
    TMP=0
    for Y in $(seq 1 $YMAX); do
        for K in 1 2; do
            TMP=$((TMP+1))
            YY=$(printf "%02d\n" "$TMP")
            IN="data/throw${X}${YY}.in"
            OUT="data/throw${X}${YY}.ans"
            ./gen "${X}.${Y}" > "$IN"
            ./std < "$IN" > "$OUT"
        done
    done
done

rm data.zip
cd data
zip ../data.zip *
cd ..