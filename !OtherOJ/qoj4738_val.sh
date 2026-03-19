#!/bin/bash
N=300
while true; do
    inp="$N\n$(seq 1 $N | shuf | tr '\n' ' ')"
    echo -e "$inp" | ./qoj4738
    code=$?
    [ $code -ne 0 ] && f="qoj4738_$(date +%s)_$code.txt" && echo -e "$inp" > "$f" && echo "RE! saved $f" && exit
done
