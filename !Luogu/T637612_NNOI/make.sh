#!/bin/bash

# 创建 data 目录
mkdir -p data

# ---------------------------
# Subtask 1: n,q ≤ 1000
# 生成 4 个数据
# ---------------------------
for i in {1..4}; do
    echo "Generating Subtask1 test $i ..."
    ./gen 1000 1000 20000 1 > data/data${i}.in $RANDOM$RANDOM
    ./std < data/data${i}.in > data/data${i}.out
done

# ---------------------------
# Subtask 2: c_i ≤ 100
# 生成 4 个数据
# ---------------------------
for i in {5..8}; do
    echo "Generating Subtask2 test $i ..."
    ./gen 40000 40000 1000000 1 > data/data${i}.in $RANDOM$RANDOM
    # 你需要在 gen 内部控制 c_i ≤ 100
    ./std < data/data${i}.in > data/data${i}.out
done

# ---------------------------
# Subtask 3: sum k_i ≤ 1000
# 生成 4 个数据
# ---------------------------
for i in {9..12}; do
    echo "Generating Subtask3 test $i ..."
    ./gen 40000 40000 1000 1 > data/data${i}.in $RANDOM$RANDOM
    ./std < data/data${i}.in > data/data${i}.out
done

# ---------------------------
# Subtask 4: g = 0
# 生成 4 个数据
# ---------------------------
for i in {13..16}; do
    echo "Generating Subtask4 test $i ..."
    ./gen 40000 40000 1000000 0 > data/data${i}.in $RANDOM$RANDOM
    ./std < data/data${i}.in > data/data${i}.out
done

# ---------------------------
# Subtask 5: Full constraints
# 生成 4 个数据
# ---------------------------
for i in {17..20}; do
    echo "Generating Subtask5 test $i ..."
    ./gen 40000 40000 1000000 1 > data/data${i}.in $RANDOM$RANDOM
    ./std < data/data${i}.in > data/data${i}.out
done

rm data.zip
cd data
zip ../data.zip *
cd ..

echo "All 20 tests generated."
