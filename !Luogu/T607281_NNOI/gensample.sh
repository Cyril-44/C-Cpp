#!/bin/bash
set -e

# 输出目录
OUTDIR="data"
mkdir -p "$OUTDIR"

# 为不同 Subtask 设置默认的 n
default_n=(0 10 200000 200000 1000 200000 200000)
seed="AAAAC3NzaC1lZDI1NTE5AAAAIAh8g0kpov1fsvH/7ncHpuwmtSkLQT97xw5X3pg4lZgf"

echo "Start generating samples..."

# 固定 mode = 0，每组生成 1 个文件 (k = 1)
mode=0
k=1

for i in {1..6}; do
    n=${default_n[$i]}

    infile="${OUTDIR}/water${i}.in"
    ansfile="${OUTDIR}/water${i}.ans"

    echo "Generating $infile (n=$n) ..."
    ./gen $i $mode $n $seed > "$infile"

    echo "Generating $ansfile ..."
    ./std < "$infile" > "$ansfile"
done

echo "Zipping files..."
cd "$OUTDIR"
zip ../samples.zip * > /dev/null
cd ..

echo "All done."