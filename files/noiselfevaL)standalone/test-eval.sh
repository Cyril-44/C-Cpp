#!/bin/bash
# 手动驱动真实 eval (WSL root 下运行): bash test-eval.sh
set -u
rm -rf /home/evaluater /home/self_evaluate/ansbackup/t1 /home/self_evaluate/result/t1
mkdir -p /home/self_evaluate/ansbackup/t1/hello /home/self_evaluate/result/t1
cp /home/cyril/hello/hello.cpp /home/self_evaluate/ansbackup/t1/hello/

echo "=== neweval.cfg ==="
cat /home/evaluate/neweval.cfg

echo "=== run eval (strace execve/chdir only) ==="
strace -f -e trace=execve,chdir,getcwd /usr/bin/eval \
    /home/evaluate \
    /home/self_evaluate/ansbackup/t1 \
    1 \
    /home/self_evaluate/result/t1 \
    hello 2>&1 | grep -vE "ENOENT|locale|gconv" | sed -n '1,60p'

echo "=== exit ==="
echo "=== result tree ==="
find /home/self_evaluate/result/t1 -type f -exec sh -c 'echo "--- $1"; cat "$1"' _ {} \;
echo "=== evaluater tree ==="
find /home/evaluater -maxdepth 3 | head -20
