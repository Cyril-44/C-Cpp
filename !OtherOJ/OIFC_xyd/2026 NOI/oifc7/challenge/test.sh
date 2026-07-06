set -e

g++ grader.cpp challenge.cpp -o grader -std=c++17 -O2

time ./grader < challenge1.in > challenge1.out
time ./matching_solver < challenge1.out > challenge2.in
time ./grader < challenge2.in > challenge2.out
