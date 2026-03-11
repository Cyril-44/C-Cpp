#!/bin/bash

# 质数列表（可自行扩展）
MODLIST=(
    10007
    1000000007
    1000000009
    998244353
    2147483647
    1811939329
    122420729
    805306457
    469762049
    1004535809
    2013265921
)

# 生成 10 组数据
for i in $(seq 0 10); do
    # N: 9e7 ~ 1e8
    N=$(( RANDOM % 9999 + 99990001 ))

    # seed: 0 ~ 2^64-1
    # 使用 /dev/urandom 生成 64 位随机数
    seed=$(od -An -N8 -tu8 < /dev/urandom | tr -d ' ')

    MOD=${MODLIST[$i]}

    # 输出到文件
    filename="data/test${i}.in"
    echo "$N"     >  "$filename"
    echo "$seed" >> "$filename"
    echo "$MOD"  >> "$filename"
    touch "data/test${i}.out"
done
