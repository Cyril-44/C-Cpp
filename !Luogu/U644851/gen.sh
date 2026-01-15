#!/bin/bash

sizes=(100 1000 10000 100000 300000 1000000 3000000 10000000 30000000 100000000)

# 生成一个 64-bit 随机数
gen_seed() {
    od -An -N8 -tu8 < /dev/urandom | tr -d ' '
}

rm data/*
mkdir -p data
for i in {1..10}; do
    idx=$(printf "%02d" $i)
    file="data/data${idx}.in"
    len=${sizes[$((i-1))]}
    seed=$(gen_seed)

    echo "Generating $file (len=$len, seed=$seed)"

    echo "$len $seed" > "$file"
    touch "data/data${idx}.out"
done

rm data.zip
zip -j data.zip interactive_lib.cpp data/*

echo "All 10 input files generated."
