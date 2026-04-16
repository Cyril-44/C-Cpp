#!/bin/bash
set -e
g++ std.cpp -o std -O2
g++ gen.cpp -o gen -O2
g++ val.cpp -o val -O2 -I/home/cyril/code/testlib

mkdir -p data samples

# 配置：次数，N上限，M上限，数值上限
generate_batch() {
    local start=$1
    local end=$2
    local max_n=$3
    local max_m=$4
    local max_v=$5
    
    for ((i=start; i<=end; i++)); do
        echo "Generating test $i..."
        ./gen $RANDOM $max_n $max_m $max_v > data/dat$i.in
        ./val < data/dat$i.in
        ./std < data/dat$i.in > data/dat$i.out
    done
}

# 1. 10% 数据 (1-2)
generate_batch 1 2 5 5 100

# 2. 40% 数据 (3-8)
generate_batch 3 8 400 800 1000

# 3. 100% 数据 (9-20)
generate_batch 9 20 1000000 100000000 1000000000000

# 4. 额外生成 3 个大样例
echo "Generating Sample 1 (Small)..."
./gen 111 5 2 3 > samples/info1.in
./std < samples/info1.in > samples/info1.ans

echo "Generating Sample 2 (Medium)..."
./gen 222 400 800 1000 > samples/info2.in
./std < samples/info2.in > samples/info2.ans

echo "Generating Sample 3 (Large)..."
./gen 333 1000000 100000000 1000000000000 > samples/info3.in
./std < samples/info3.in > samples/info3.ans

echo "All done! Data is in ./data"

rm std gen val