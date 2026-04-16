#!/bin/bash

# 设置失败即退出
set -e

mkdir -p data
echo "正在编译程序..."
g++ std.cpp -o std -O2
g++ gen.cpp -o gen -O2
g++ val.cpp -o val -O2

echo "开始处理 20 个测试点..."

for i in {1..20}
do
    infile="data/data$i.in"
    ansfile="data/data$i.ans"
    
    # 1. 生成数据
    ./gen $i > $infile
    
    # 2. 自动验证数据合法性
    if ./val $i < $infile; then
        echo -n "测试点 $i: [Val Passed] "
    else
        echo "测试点 $i: [Val Failed!] 停止生成。"
        exit 1
    fi
    
    # 3. 生成答案
    ./std < $infile > $ansfile
    echo "[Ans Generated]"
done

echo "--------------------------------------"
echo "所有数据处理完毕，存储在 data/ 目录下。"

rm std gen val