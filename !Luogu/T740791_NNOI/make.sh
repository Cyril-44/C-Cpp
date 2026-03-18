#!/bin/bash
set -e
OUTDIR=data
rm -rf data/
mkdir -p "$OUTDIR"

# 定义 5 个 sub，每个 sub 有一个 modelist
# 每个元素是 "tp:mode1,mode2,mode3"
subs=(
    "1:1,2,3,4,4,5,-1,-1"
    "2:1,2,3,4,4,4,4,4,4,5,-1,-1"
    "3:1,2,3,4,4,4,4,5,-1,-1"
    "4:1,2,3,4,4,4,4,5,-1,-1"
    "5:1,2,3,4,4,4,4,5,-1,-1"
)

# 遍历 sub
idx=1
for i in "${!subs[@]}"; do
    entry="${subs[$i]}"

    # 拆分 "tp:mode1,mode2,mode3"
    tp="${entry%%:*}"
    modelist="${entry#*:}"

    # 将 modelist 按逗号拆成数组
    IFS=',' read -r -a modes <<< "$modelist"

    # 遍历 modelist
    for j in "${!modes[@]}"; do
        mode="${modes[$j]}"
        infile="$OUTDIR/data${idx}.in"
        ansfile="$OUTDIR/data${idx}.ans"

        echo "Generating $infile  (tp=$tp, mode=$mode)"
        ./gen "$tp" "$mode" > "$infile"
        ./std < "$infile" > "$ansfile"
        ./val "$infile" "$tp" < "$infile"
        ((idx++))
    done
done

cd data
zip ../data.zip *
cd ..
