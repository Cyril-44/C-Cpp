#!/bin/bash

# 设置失败即退出
set -e

mkdir -p data samples
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
echo "所有数据生成完毕，开始挑选大样例..."

# 四组
GROUP_A=(1 2 3 4)
GROUP_B=(5 6 7 8)
GROUP_C=(13 14 15 16)
GROUP_D=(9 10 11 12 17 18 19 20)

pick_random() {
    local arr=("$@")
    local len=${#arr[@]}
    local idx=$((RANDOM % len))
    echo ${arr[$idx]}
}

A=$(pick_random "${GROUP_A[@]}")
B=$(pick_random "${GROUP_B[@]}")
C=$(pick_random "${GROUP_C[@]}")
D=$(pick_random "${GROUP_D[@]}")

echo "大样例选择如下："
echo "  组 A: data$A"
echo "  组 B: data$B"
echo "  组 C: data$C"
echo "  组 D: data$D"

echo "--------------------------------------"
echo "开始重新运行生成器生成大样例..."

cnt=0
for x in $A $B $C $D; do
    cnt=$((cnt+1))

    sample_in="samples/ex$cnt.in"
    sample_ans="samples/ex$cnt.ans"

    # 重新运行生成器
    ./gen $x > $sample_in

    # 再验证
    if ./val $x < $sample_in; then
        echo -n "大样例 $x: [Val Passed] "
    else
        echo "大样例 $x: [Val Failed!] 停止生成。"
        exit 1
    fi

    # 再跑 std
    ./std < $sample_in > $sample_ans
    echo "[Ans Generated]"
done

echo "--------------------------------------"
echo "大样例已重新生成，存储在 samples/ 目录下。"

rm std gen val
cp ../tuackCodeTheme.tmTheme .
cp ../NNOI_Default_template.typ .default.temp.typ
echo "#include \"statement.typ\"" >> .default.temp.typ
pandoc statement.typ -o statement.tex --highlight-style=none
typst compile .default.temp.typ statement.pdf
cp ../NNOI_Default_template.typ .default.temp.typ
echo "#include \"solution.typ\"" >> .default.temp.typ
pandoc solution.typ -o solution.tex --highlight-style=none
typst compile .default.temp.typ solution.pdf

name=$(basename "$PWD")
clean=${name%_NNOI}
rm -f ${clean}_data.zip
zip -r ${clean}_data.zip data/ samples/ statement.tex statement.pdf solution.tex solution.pdf

rm statement.pdf statement.tex solution.pdf solution.tex tuackCodeTheme.tmTheme .default.temp.typ
rm -rf data/ samples/
