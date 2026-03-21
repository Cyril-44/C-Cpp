#!/bin/bash

# ==========================================
# 配置区
# ==========================================

# 每个 Subtask 生成多少份数据文件
FILES_SUB1=5
FILES_SUB2=5
FILES_SUB3=5

# 每份数据包含多少组
T=20

# mode 范围（0 ~ MODE_MAX）
MODE_MAX=7

# 输出目录
OUTDIR="data"

# ==========================================
# 工具函数
# ==========================================

rand64() {
    od -An -N8 -tu8 < /dev/urandom | tr -d ' '
}

gen_one_file() {
    local OUTFILE=$1
    local MAXN=$2

    echo "$T" >> "$OUTFILE"

    for ((i=1; i<=T; i++)); do
        n=$(( RANDOM % MAXN + 1 ))
        mode=$(( RANDOM % (MODE_MAX + 1) ))
        seed=$(rand64)
        echo "$n $mode $seed" >> "$OUTFILE"
    done
}

# ==========================================
# 主流程
# ==========================================

mkdir -p "$OUTDIR"

echo "Generating Subtask 1 (N <= 7)..."
for ((f=1; f<=FILES_SUB1; f++)); do
    IN="$OUTDIR/sub1_data$f.in"

    echo "1" > "$IN"
    gen_one_file "$IN" 7
done

echo "Generating Subtask 2 (N <= 50)..."
for ((f=1; f<=FILES_SUB2; f++)); do
    IN="$OUTDIR/sub2_data$f.in"

    echo "2" > "$IN"
    gen_one_file "$IN" 50
done

echo "Generating Subtask 3 (N <= 256)..."
for ((f=1; f<=FILES_SUB3; f++)); do
    IN="$OUTDIR/sub3_data$f.in"

    echo "3" > "$IN"
    gen_one_file "$IN" 256
done

echo "All data generated in $OUTDIR/"
