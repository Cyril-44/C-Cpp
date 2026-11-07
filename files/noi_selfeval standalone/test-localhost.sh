#!/bin/bash
# 单机 127.0.0.1 直连复现验证 (root 运行)
set -u
PKG="/mnt/d/Hardsoft/NOI SelfEval/re/standalone"
ap() { python3 "$PKG/arbiter-plus.py" --yes --exec "$@"; }

echo "== [1] aclient 直接本机启动 (无 netns) =="
pkill -x aclient 2>/dev/null; sleep 0.3
setsid "$PKG/assets/aclient" --nodaemon >/tmp/aclient.log 2>&1 &
sleep 1
ss -uln | grep -q 7730 && echo "  UDP 7730 (命令) UP"
ss -tlnp 2>/dev/null | grep -q 7731 && echo "  TCP 7731 (文件, aclient FServer) UP"

echo "== [2] 落盘目标父目录 (FServer 不建目录) =="
mkdir -p /srv/f /srv/r /srv/b

echo "== [3] 管理端连 127.0.0.1 =="
ap "clients add 127.0.0.1" 2>&1 | grep -vE "^$"
ap 'send -s "echo -i -p /home/srv -d 1" 127.0.0.1' 2>&1 | grep -E "OK|ERR"
ap 'send -s "userset -s -p pw123 hello" 127.0.0.1' 2>&1 | grep -E "OK|ERR"

echo "== [4] mupload 发题 (借道 aclient 自己的 FServer 落盘) =="
ap "mupload /home/evaluate/test1.in /home/evaluate/probe.in 127.0.0.1" 2>&1 | grep -E "result"
ls -la /home/evaluate/probe.in 2>/dev/null | awk '{print "  落盘:", $NF, $5"B"}'

echo "== [5] pickans + eval =="
ap 'send -s -t 30 "pickans /srv/b" 127.0.0.1' 2>&1 | grep -E "OK|ERR"
ap 'send -s -t 90 "eval -e /srv/r /home/evaluate hello plus" 127.0.0.1' 2>&1 | grep -E "OK|ERR"

echo "== [6] file -s 取回成绩单 (回 127.0.0.1 = 本机落盘) =="
for p in hello plus; do
  ap "send -s -t 15 \"file -s -m 0644 /home/evaluate/result/$p.rlt /srv/f/$p.rlt 127.0.0.1\" 127.0.0.1" 2>&1 | grep -E "OK|ERR"
done

echo "== [7] 成绩单 =="
for p in hello plus; do
  echo "--- /srv/f/$p.rlt:"; cat "/srv/f/$p.rlt" 2>/dev/null || echo "(missing)"
done
