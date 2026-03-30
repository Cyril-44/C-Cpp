#!/bin/bash

# 需要存在 ./bf 和 ./std
if [ ! -x ./bf ]; then
    echo "Error: ./bf not found or not executable"
    exit 1
fi
if [ ! -x ./std ]; then
    echo "Error: ./std not found or not executable"
    exit 1
fi

# 临时文件
IN=tmp_input.txt
OUT1=tmp_bf.txt
OUT2=tmp_std.txt
# echo 1 > $IN
# echo 1000000 >> $IN
# for ((i=1; i<=1000000; i++)) do
# echo $RANDOM >> $IN;
# done 
# exit 0

echo $((5*5*5*5*5*5*5*5)) > $IN

# 枚举所有 a[1..8] ∈ [1..8]
for a1 in {1..5}; do
for a2 in {1..5}; do
for a3 in {1..5}; do
for a4 in {1..5}; do
for a5 in {1..5}; do
for a6 in {1..5}; do
for a7 in {1..5}; do
for a8 in {1..5}; do

    echo "8 $a1 $a2 $a3 $a4 $a5 $a6 $a7 $a8" >> $IN

done; done; done; done; done; done; done; done
./bf < $IN > $OUT1
./std < $IN > $OUT2
# 比较
if ! diff -q $OUT1 $OUT2 >/dev/null; then
    echo "===================="
    echo "Mismatch found!"
    echo "===================="
    exit 1
fi
echo "All $cnt tests passed!"
