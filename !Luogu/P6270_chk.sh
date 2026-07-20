#!/bin/bash
set -e

for ((i=1;;i++)) do 
    ./P6270_gen > P6270.in
    ./P6270 < P6270.in > P6270.out
    ./P6270_checker P6270.in P6270.out P6270.out
done