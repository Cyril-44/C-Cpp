#!/usr/bin/bash
for ((t=1; t<=100; t++)) {
    echo Test $t;
    ./gen >miao.in;
    \time -f "Time: %e, Result: %x." ./miao
    ./checker miao.in miao.out /dev/null;
}