#!/bin/bash
set -e
g++ std.cpp -o std -O2
g++ gen.cpp -o gen -O2 -I/home/cyril/code/testlib
g++ val.cpp -o val -O2 -I/home/cyril/code/testlib
OUTDIR=data
SAMP_DIR=samples
rm -rf "$OUTDIR" "$SAMP_DIR"
mkdir -p "$OUTDIR" "$SAMP_DIR"

# 定义 5 个 sub
subs=(
    "1:1,2,3,4,4,5,-1,-1"
    "2:1,2,3,4,4,4,4,4,4,5,-1,-1"
    "3:1,2,3,4,4,4,4,5,-1,-1"
    "4:1,2,3,4,4,4,4,5,-1,-1"
    "5:1,2,3,4,4,4,4,5,-1,-1"
)

idx=1
for i in "${!subs[@]}"; do
    entry="${subs[$i]}"
    tp="${entry%%:*}"
    modelist="${entry#*:}"

    IFS=',' read -r -a modes <<< "$modelist"

    # 1. 生成常规测试数据 (data/data{idx}.in)
    for j in "${!modes[@]}"; do
        mode="${modes[$j]}"
        infile="$OUTDIR/data${idx}.in"
        ansfile="$OUTDIR/data${idx}.ans"

        echo "Generating $infile (tp=$tp, mode=$mode)"
        ./gen "$tp" "$mode" > "$infile"
        ./std < "$infile" > "$ansfile"
        ./val "$infile" "$tp" < "$infile"
        ((idx++))
    done

    # 2. 生成该 sub 对应的样例数据 (samples/ex{tp}.in)
    # 固定使用 mode = -1
    ex_infile="$SAMP_DIR/ex${tp}.in"
    ex_ansfile="$SAMP_DIR/ex${tp}.ans"
    
    echo "Generating Sample: $ex_infile (tp=$tp, mode=-1)"
    ./gen "$tp" -1 > "$ex_infile"
    ./std < "$ex_infile" > "$ex_ansfile"
    ./val "$ex_infile" "$tp" < "$ex_infile"

done

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