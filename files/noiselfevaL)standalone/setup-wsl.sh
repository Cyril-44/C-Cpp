#!/bin/bash
# =====================================================================
# ArbiterPlus 模拟考场一键部署 (WSL2 Ubuntu 20.04, 以 root 运行)
# 用法: sudo bash setup-wsl.sh [/mnt/e]     # 参数=NOI Linux ISO 挂载盘(可选)
# 资产优先级: re/eval_bin(已抽出) > /mnt/e squashfs(现场抽) > 跳过
# =====================================================================
set -u
SRC="/mnt/d/Hardsoft/NOI SelfEval"
STAGE=$SRC/re
ISO=${1:-/mnt/e}
CONTEST_USER=${CONTEST_USER:-$(ls /home | grep -vE 'evaluater|lost' | head -1)}

echo "== [1/6] 评测器 /usr/bin/eval =="
if [ -f "$STAGE/eval_bin" ]; then
    cp "$STAGE/eval_bin" /usr/bin/eval
elif [ -e "$ISO/casper/filesystem.squashfs" ]; then
    mkdir -p "$STAGE/.extract"
    unsquashfs -d "$STAGE/.extract/root" -f "$ISO/casper/filesystem.squashfs" \
        'usr/bin/eval' 'usr/local/arbiter/local/filter' >/dev/null 2>&1
    cp "$STAGE/.extract/root/usr/bin/eval" /usr/bin/eval
    cp "$STAGE/.extract/root/usr/local/arbiter/local/filter/fulltext_e" "$STAGE/fulltext_e" 2>/dev/null
    rm -rf "$STAGE/.extract"
else
    echo "!! 未找到 eval (re/eval_bin 或 ISO), 跳过"; exit 1
fi
chmod 755 /usr/bin/eval
[ -f "$STAGE/fulltext_e" ] && cp "$STAGE/fulltext_e" /home/evaluate_check 2>/dev/null

echo "== [2/6] 用户与目录 =="
id evaluater >/dev/null 2>&1 || useradd -m -s /bin/bash evaluater
getent group noiuser >/dev/null || groupadd -g 2000 noiuser || true
mkdir -p /home/evaluater/testprograms /var/log/arbiter /home/self_evaluate
chown -R evaluater:evaluater /home/evaluater 2>/dev/null || true
chmod 777 /var/log/arbiter

echo "== [3/6] 评测配置 (示例题 hello) =="
mkdir -p /home/evaluate/data/hello
cp "$STAGE/fulltext_e" /home/evaluate/fulltext_e && chmod 755 /home/evaluate/fulltext_e
printf '5 1\nhello\n' > /home/evaluate/selfeval.cfg
# neweval.cfg 格式 (逆向自 /usr/bin/eval ReadyForEvaluate/TaskTest/runUnderCtrl):
#   首行  : <名称> <int 兼容aclient pickans> <题数>
#   题行  : 题名 stdin重定向 stdout重定向 picklimitKB 输入文件 选手输出文件 比对器 可执行名 时限 内存MB outtype ccl数
#   ccl行 : <源码后缀> <编译命令 (%i=源文件 %o=输出)> ×ccl数
#   测试点: <点数> 然后 <输入文件 答案文件 分值> ×点数
cat > /home/evaluate/neweval.cfg <<'EOF'
contest 0 1
hello 1 1 100 hello_std.in hello.out fulltext_e hello 1.0 0 0 1
cpp g++ -O2 -o %o %i
1
test1.in test1.ans 1.0
EOF
printf 'hello world\n' > /home/evaluate/test1.in
printf 'hello world\n' > /home/evaluate/test1.ans
cp /home/evaluate/test1.in  /home/evaluate/data/hello/
cp /home/evaluate/test1.ans /home/evaluate/data/hello/
chmod -R a+rX /home/evaluate

echo "== [4/6] 编译 arbiter-server =="
mkdir -p /root/arbiter-src 2>/dev/null || true
BUILD=/home/$CONTEST_USER/arbiter-src
[ -d "$BUILD" ] || BUILD=/root/arbiter-src
cd "$BUILD" && make >/dev/null 2>&1 || { echo "!! 请先运行: cd ~/arbiter-src && make"; }

echo "== [5/6] 选手源码示例 =="
mkdir -p /home/$CONTEST_USER/hello
printf '#include <cstdio>\nint main(){char b[256];if(fgets(b,256,stdin))printf("hello world\\n");return 0;}\n' \
    > /home/$CONTEST_USER/hello/hello.cpp
chown -R $CONTEST_USER:$CONTEST_USER /home/$CONTEST_USER/hello

echo "== [6/6] 完成 =="
cat <<EOF

启动方式 (两个终端):
  终端A: sudo /root/../home/*/arbiter-src 不好记? 直接:
         sudo ~$CONTEST_USER/arbiter-src/arbiter-server --local   # 自测服务器(7729->7732)
  终端B: sudo "$SRC/bclient" --nodaemon                          # 自测客户端(7732)

arbiter-server 控制台:
  send -s -t 5 "tasknames"                       # 查题目/剩余次数
  send -s -t 60 "selfeval -u $CONTEST_USER hello" # 触发真实评测
  cat /home/self_evaluate/result/*/*/hello.rlt    # 看结果
EOF
