#!/bin/bash
# 端到端验收脚本 (WSL Ubuntu 20.04 内运行)
# 用法: bash test-e2e.sh
set -u
SRC=/mnt/d/Hardsoft/NOI\ SelfEval
WDIR=/home/cyril/arbiter-src

cd "$WDIR" || exit 1
# 同步最新源码 (Windows 侧 -> WSL); 注意必须在 cd 之后, 否则相对目标错位
cp -f "$SRC/re/standalone/common/"*.h "$SRC/re/standalone/common/"*.cpp common/
cp -f "$SRC/re/standalone/server/"*.h "$SRC/re/standalone/server/"*.cpp server/
cp -f "$SRC/re/standalone/arbiter-server.cpp" .
make clean >/dev/null
make 2>&1 | grep -E "error|Error" && exit 1
echo "[1] build ok: $(ls -la arbiter-server | awk '{print $5}') bytes"

pkill -x bclient 2>/dev/null
sleep 0.5
setsid "$SRC/bclient" --nodaemon >/tmp/bclient-run.log 2>&1 &
sleep 1
if ss -uln | grep -q 7732; then rm -rf /home/evaluater /home/self_evaluate/result/* 2>/dev/null
echo "[2] bclient listening UDP 7732"; else
  echo "[2] FAIL: bclient not listening"; tail -5 /tmp/bclient-run.log; exit 1; fi

echo "[3] tasknames:"
./arbiter-server --local -c 'send -s -t 5 "tasknames" 127.0.0.1' 2>/dev/null | \
  grep -vE '^N[0-9]|^>>> exit' | grep -v '^$'

echo "[4] selfeval (real /usr/bin/eval):"
./arbiter-server --local -c 'send -s -t 60 "selfeval -u cyril hello" 127.0.0.1' 2>/dev/null | \
  grep -vE '^N[0-9]' | grep -v '^$'

echo "[5] result files:"
find /home/self_evaluate/result -name '*.rlt' 2>/dev/null | while read f; do
  echo "--- $f"; cat "$f"
done

echo "[6] bclient tail:"
grep -vE 'waiting packet' /tmp/bclient-run.log | tail -12

pkill -x bclient 2>/dev/null
echo DONE
