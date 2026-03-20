#!/bin/bash
set -e

# 输出目录
OUTDIR="data"
mkdir -p "$OUTDIR"

# 6×5 的数量矩阵（你可以按需修改）
# cnt[i][j] = Subtask i, mode j 要生成多少个
cnt=(
    "0 0 0 0 0"   # 占位：从下标1开始
    "3 2 1 0 0"   # Subtask 1
    "5 5 5 5 5"   # Subtask 2
    "2 2 2 2 2"   # Subtask 3
    "4 4 4 4 4"   # Subtask 4
    "1 1 1 1 1"   # Subtask 5
    "6 6 6 6 6"   # Subtask 6
)

# 可选：为不同 subtask 设置默认 n
default_n=(0 10 200000 200000 1000 200000 200000)
seed=6bZVzGBTn3CStisLIOiMcS05eLIdz5/GB1hRU47F3zs=
echo "Start generating..."

for i in {1..6}; do
    for j in {1..5}; do
        # 取 cnt[i][j]
        row=(${cnt[$i]})
        num=${row[$((j-1))]}
        for ((k=1; k<=num; k++)); do
            n=${default_n[$i]}

            infile="${OUTDIR}/data${i}${j}${k}.in"
            ansfile="${OUTDIR}/data${i}${j}${k}.ans"

            echo "Generating $infile ..."
            ./gen $i $j $n $seed > "$infile"
            echo "Generating $ansfile ..."
            ./std < "$infile" > "$ansfile"

        done
    done
done

echo "Zipping files..."
cd data
zip ../data.zip * > /dev/null
cd ..

echo "All done."
