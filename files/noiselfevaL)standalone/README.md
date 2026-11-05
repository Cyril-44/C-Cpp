# arbiter-server —— 独立 ArbiterPlus 服务器 (WSL2 可编译运行)

从 `selfeval/aclient/bclient` 三个二进制逆向重建的**无 Qt 依赖**独立服务器，
等价于原 selfeval GUI 内嵌 Server + 控制台入口。已在本机 WSL2 Ubuntu 20.04
完成**真实评测器端到端验收**（编译→沙箱运行→filter 比对→.rlt 产出→结果回传）。

```
┌ WSL2 Ubuntu 20.04 ─────────────────────────────────────────────┐
│ arbiter-server(本工程) ──UDP 7729→7732──▶ bclient(原版二进制)    │
│   REPL: send -s "selfeval -u user 题" 127.0.0.1                │
│            bclient: 备份源码 → /usr/bin/eval(真实评测器,取自     │
│            NOI Linux ISO) → uid60000 沙箱运行 → fulltext_e 比对 │
│            → .rlt → "&0 <时间戳>" 回传 → 控制台显示得分          │
└────────────────────────────────────────────────────────────────┘
```

## 一、部署（一键）

```bash
# WSL2 Ubuntu 20.04 内 (g++/make 需已装: sudo apt install g++ make)
sudo bash /mnt/d/Hardsoft/NOI\ SelfEval/re/standalone/setup-wsl.sh
```
脚本自动：装 `/usr/bin/eval` 与 `fulltext_e`（优先用 `re/eval_bin` 已抽出的资产，
否则从 E:\ NOI Linux ISO 的 squashfs 现场抽取）→ 建用户/目录 → 写示例题 `hello`
→ 建选手源码。

## 二、编译

```bash
cd ~/arbiter-src && make          # 源码同步自 re/standalone/
./arbiter-server --help
```
产物 `arbiter-server`（~4MB，g++ -O2 -pthread，无任何第三方依赖）。

## 三、运行

**两个终端（都建议 root——真实部署里客户端/服务都是 root 服务）：**

```bash
# 终端A: 自测服务器 (本地模式 7729→7732, 驱动 bclient)
sudo ~/arbiter-src/arbiter-server --local

# 终端B: 自测客户端 (原版二进制)
sudo /mnt/d/Hardsoft/NOI\ SelfEval/bclient --nodaemon
```

**arbiter-server 控制台命令**（REPL 或 `-c '单条命令'`）：

```bash
# 查题目列表+最大评测次数  (bclient 读 /home/evaluate/selfeval.cfg)
send -s -t 5 "tasknames" 127.0.0.1

# 触发真实评测  (备份源码→eval→沙箱运行→比对→.rlt)
send -s -t 60 "selfeval -u <用户名> hello" 127.0.0.1

# 查看结果
cat /home/self_evaluate/result/<时间戳>/hello.rlt

# 完整模式(驱动 aclient, 含文件通道 TCP 7731):
sudo ~/arbiter-src/arbiter-server            # 注意: 7729→7730
send -s "status" <aclient-ip>
send -s "sh -e \"id\"" <aclient-ip>
mupload -m 0755 <本地文件> <远端路径> <客户端ip>
# ⚠ 切勿发送 shutdown —— 会真的关机!
```

验收示例（本机实测输出）：
```
>>> send -s -t 5 "tasknames" 127.0.0.1
  [result] 127.0.0.1: res=5 info=hello
>>> send -s -t 60 "selfeval -u cyril hello" 127.0.0.1
  [result] 127.0.0.1: res=0 info=1793899883
$ cat /home/self_evaluate/result/1793899883/hello.rlt
{hello 100 Compile successfully
101 1.000000 0.003 Right Output!!!
}102 1.000000
```

## 四、文件与目录约定（全部实测验证）

| 路径 | 作用 |
|---|---|
| `/home/evaluate/selfeval.cfg` | 首行 `最大次数 题数`，次行题名（bclient tasknames 读） |
| `/home/evaluate/neweval.cfg` | 评测配置（eval 读，格式见下） |
| `/home/evaluate/test1.in/.ans` | 测试数据（**平铺**，非 data/ 子目录） |
| `/home/evaluate/fulltext_e` | 比对器（也可用 /usr/local/arbiter/local/filter/ 其它 filter） |
| `/home/<用户>/<题>/<题>.cpp` | 选手源码（GUI/命令均校验存在） |
| `/home/self_evaluate/ansbackup/<ts>/<题>/` | bclient 备份的源码 |
| `/home/self_evaluate/result/<ts>/<题>.rlt` | 评测结果 |
| `/home/evaluater/` | eval 沙箱（每轮 rm 重建；testprograms/config） |
| `/var/log/arbiter/*.log` | arbiter-server / evalclient 日志 |

## 五、neweval.cfg 格式（逆向 eval 的 ReadyForEvaluate/TaskTest/runUnderCtrl 实测）

```
<名称> <int> <题数>                        ← 首行 3 字段（兼容 aclient pickans 的 "%s%s"+题数）
题名 <stdin重定向1/0> <stdout重定向1/0> <picklimitKB> <输入文件> <选手输出文件> <比对器> <可执行名> <时限s> <内存MB> <outtype> <ccl数>
<源码后缀> <编译命令 %i=源文件 %o=输出>      ← ×ccl数
<测试点数>
<输入文件> <答案文件> <分值>                 ← ×测试点数
```

实测最小示例（hello, g++）：
```
contest 0 1
hello 1 1 100 hello_std.in hello.out fulltext_e hello 1.0 0 0 1
cpp g++ -O2 -o %o %i
1
test1.in test1.ans 1.0
```

要点（踩坑记录）：
- 第 4/5 字段开 IO 重定向后，输入文件实际名为 `<第5字段>`，选手程序跑在
  uid=60000/gid=2000 沙箱（fork 内 mv→stdin、stdout→O_TRUNC open→setuid→execv）
- 第 8 字段 = 选手**可执行文件名**（编译产物名，漏填会 "Can not find source/差异"）
- 比对器第 4 参后输出分数到 /tmp/_eval.score；`%s%d` 测试点行文件需平铺于 config
- .rlt 行格式：`{题 100 Compile successfully` / `得分 用时 状态 文本` / `}总分 时间`

## 六、工程结构

```
standalone/
├── common/          ← 逆向重建的 c-s/common（编译修复版）
├── server/          ← 逆向重建的 c-s/server（Server/SendCmd/MUpload/SelfEvalResultCmd）
├── arbiter-server.cpp ← 控制台入口（REPL + -c 单发 + --local 本地模式）
├── Makefile
├── setup-wsl.sh     ← 一键部署
├── test-e2e.sh      ← 端到端验收（tasknames + selfeval 真实评测闭环）
└── test-eval.sh     ← eval 单独调试脚本
资产: ../eval_bin（NOI 官方评测器, 从 E:\ ISO 抽出）、../fulltext_e（比对器）
```

## 七、已知边界

1. REPL/`-c` 是新增入口（原系统由 Qt GUI 触发 Server::execute，逻辑等价）。
2. 比赛完整模式中 `file/mupload` 已实现基础传输；checksum 校验/DirCache 目录
   映射为简化版（见 fserver.cpp 注释）；`selfevalresult` 走控制台通知。
3. bclient 里编译进但未注册的 clientcmd 命令（userset/eval 等）在 bclient 上
   不可用——如需比赛命令集用 **aclient + 完整模式 server**。
4. `shutdown` 命令真实有效（system("shutdown -h now")），永远别对真机发。
