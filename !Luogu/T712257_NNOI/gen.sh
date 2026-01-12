#!/bin/bash

set -e

# 输出目录
OUTDIR="tests"
rm -f -r "$OUTDIR"
mkdir -p "$OUTDIR"

# 生成随机 seed（64bit）
gen_seed() {
    od -An -N8 -tu8 < /dev/urandom | tr -d ' '
}

# 你可以在这里配置要生成的测试规模
# 格式： n treeType valueType
configs=(
    # Subtask 1: n <= 1000
    "200 1 1"
    "500 1 1"
    "1000 1 1"
    "1000 4 1"
    "1000 5 1"

    # Subtask 2: s_i <= 100
    "20000 1 2"
    "50000 1 2"
    "80000 4 2"
    "100000 5 2"
    "150000 1 2"

    # Subtask 3: chain
    "1000 2 1"
    "50000 2 1"
    "100000 2 1"
    "200000 2 1"
    "200000 2 3"

    # Subtask 4: full constraints, hard cases
    "200000 1 1"
    "200000 1 3"
    "200000 4 3"
    "200000 5 4"
    "200000 1 5"
)


echo "Generating test data..."

id=1
for cfg in "${configs[@]}"; do
    read -r n ttype vtype <<< "$cfg"
    seed=$(gen_seed)

    echo "Test $id: n=$n treeType=$ttype valueType=$vtype seed=$seed"

    ./gen "$n" "$ttype" "$vtype" "$seed" > "$OUTDIR/data$id.in"
    ./std_bot < "$OUTDIR/data$id.in" > "$OUTDIR/data$id.out"
    id=$((id+1))
done

echo "Packing zip..."
rm -f tests.zip
cd $OUTDIR
zip -r ../tests.zip . >/dev/null
cd ..

echo "Done. Output: tests.zip"
