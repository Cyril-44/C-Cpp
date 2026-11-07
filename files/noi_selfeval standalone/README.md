# ArbiterPlus 考场套件 —— standalone package

本包提供 **arbiter-plus 管理端**（单文件 Python，管理运行原版 aclient 的选手机），
内含 `assets/` 静态资产（原版三二进制 + 官方评测器 eval + 30 个比对器 filter），自包含。
全部协议与文件通道逆向自 ArbiterPlus 二进制；评测链已实测打通
（发题 → 收卷 → 真实 eval 终评 → 成绩单回传，`Right Output` 全绿）。

```
standalone/                      ← 单一自包含 package
├── arbiter-plus.py              管理端（单文件, 纯标准库, CLI + WebUI）
├── README.md
└── assets/                      静态资产（从 NOI Linux ISO 抽出, 不再依赖 ISO）
    ├── aclient / bclient / selfeval   原版三个二进制
    ├── eval                     官方评测器（仅依赖 libc）
    └── filter/                  30 个官方比对器
```

---

## 一、ArbiterPlus 三程序简介

| 程序 | 角色 | 端口 |
|---|---|---|
| **selfeval** | 选手机自测 GUI（Qt），内嵌本地服务器 | UDP 7729→7732 |
| **bclient** | 自测客户端（守护进程，被 selfeval 驱动） | UDP 7732 |
| **aclient** | 比赛客户端（守护进程，被服务器远程管理） | UDP 7730 + TCP 7731 |

**selfeval**（GUI，无独立命令行；内嵌服务器仅注册 version/help/send）：
- 显示题目列表/剩余自测次数（来自 bclient 的 tasknames）
- 弹选题对话框，校验 `/home/<用户>/<题>/<题>.{c,cpp,pas}` 存在
- 经内嵌服务器向本机 bclient 发送 `selfeval` 命令，等待结果
- 解析 `/home/self_evaluate/result/<时间戳>/*.rlt` 显示成绩表格

**bclient**（4 个命令）：
- `version` — 返回版本串
- `help [-v] [command]` — 命令帮助
- `tasknames` — 读 `/home/evaluate/selfeval.cfg`，返回（最大自测次数, 题目名列表）
- `selfeval -u <用户> [题...]` — 备份源码到 `ansbackup/<时间戳>/` → 调 `/usr/bin/eval` 评测 → 校验 `.rlt` → 尽力发 `selfevalupload` 通知 → 返回时间戳

**aclient**（14 个命令）：
- `echo [-i] [-p home] [-d day]` — 回显；`-i` 登记服务器（IP/工作目录/日次）
- `help [-v] [command]` — 命令帮助
- `version` — 返回版本串
- `status` — 查询状态（启动时间/评测路径/命令历史/文件历史）
- `ps` — 列出正在运行的异步命令
- `sh [-e] "命令"` — 执行 shell 命令，`-e` 回显输出
- `userset [-s] [-p 密码] [-r] [-f] 用户名` — 建/删比赛用户（noiuser 组），`-s` 设为活动用户 `$ACTIVEUSER`
- `file [-s] [-m mode] [-u owner] [-v] 本地 远端 目标` — 文件传输（`-s` 推送，否则拉取），走对端 TCP 7731
- `eval [-e result-path] 评测目录 [题...]` — 终评：调 `/usr/bin/eval` → 校验 `.rlt` → `file -s` 回传成绩
- `pickans <上传目录>` — 收卷：把 `/home/$ACTIVEUSER/<题>/*.{cpp,c,pas}` 备份到 `/home/ansbackup` 并上传
- `fcheck <路径> <md5>` — 校验客户端文件
- `clearenv [路径...]` — 清场（删 /home 下指定路径 + 全部 noiuser 用户）
- `shutdown [-r]` — 关机（`-r` 重启）**危险！**
- `selfevalupload -t <时间戳>` — 上传自测结果并通知服务器（内部命令）

---

## 二、arbiter-plus 实现了什么

替代原"GUI 内嵌服务器 + 考场操作员"的完整服务端，**面向 aclient 管理**：

- **UDP 命令引擎**（7729→7730）：6 字节包头协议，code 位（01 请求/04 等响应/08 抓取客户端 stdout/20 最终结果/40 要回执）；`send` 支持多目标、超时收集、进度与结果回显
- **TCP 文件接收器**（7731）：`put→ok→数据→echo` 帧协议，接收 eval 成绩回传与 pickans 收卷，落盘 `./received/<远端路径>`
- **mupload 文件分发**：等价原 MUploadCmd——主动连客户端 7731 推送文件（发题通道）
- **selfevalresult 通知监听**：客户端主动上报的自测结果实时提示
- **客户端台账**：登记/删除/广播扫描发现（`clients` / `scan`）
- **CLI**：Tab 自动补全（14 个客户端命令 + 用法提示 + 目标 IP 补全）、高危命令强制确认（userset/clearenv/shutdown）
- **WebUI**（`--web`）：机器列表、一键 status/ps/echo、自定义命令输入框（datalist 补全）、高危命令拦截
- **批处理模式**：`--exec "..."` 单发命令（脚本/CI），`--yes` 跳过确认

```bash
python3 arbiter-plus.py                # CLI
python3 arbiter-plus.py --web          # CLI + WebUI (http://127.0.0.1:8080)
python3 arbiter-plus.py --yes --exec 'send -s "status" all'
```

CLI 内建命令：`send` / `clients` / `scan` / `mupload` / `received` / `webui` / `quit`；
客户端命令可直接敲（如 `status all`、`eval -e /srv/r /home/evaluate hello plus`）。

### 标准考场流程（实测路径）

```
clients add <ip>
send -s "echo -i -p /home/srv -d 1" <ip>                # ① 登记服务器
send -s "userset -s -p pw123 AH-01" <ip>                # ② 建比赛账号(源码在 /home/AH-01/<题>/)
mupload <试题文件> /home/evaluate/<同名> <ip>            # ③ 分发试题(逐文件)
send -s "pickans /srv/backup" <ip>                      # ④ 收卷
send -s -t 90 "eval -e /srv/r /home/evaluate <题...>" <ip>   # ⑤ 终评
send -s "file -s -m 0644 /home/evaluate/result/<题>.rlt /srv/f/<题>.rlt <服务器IP>" <ip>   # ⑥ 取回成绩
cat ./received/srv/f/<题>.rlt
```

### 单机复现（自己连自己，推荐 127.0.0.1 方式）

aclient 直接本机启动，管理端连 `127.0.0.1`。文件通道**借道 aclient 自己的 FServer**：
它上传 `file -s ... 127.0.0.1` 时连到的 7731 就是自己的文件服务，文件照常落盘本机
（管理端启动时打印一行 `TCP 7731 绑定失败` 属预期——文件不经管理端接收器）。
两个注意点：落盘目标的父目录要先 `mkdir`（FServer 不建目录）；成绩单落在命令里指定的远端路径。
完整可照抄流程见包内 `test-localhost.sh`（实测两题全绿）。

若要模拟"多机考场"（管理端 10.10.0.1 ↔ 选手机 10.10.0.2），再用 netns 隔离：
`ip netns add c1; ip link add veth0 type veth peer name veth1; ip link set veth1 netns c1; ...`
真实多机部署则直接 `clients add <对方IP>`，无需任何特殊处理。

---

## 三、题目配置格式（aclient / eval 用：neweval.cfg）

eval（及 aclient 的 pickans）读取 `/home/evaluate/neweval.cfg`：

```
<名称> <int> <题数>                          ← 首行 3 字段（两种解析器兼容）
题行 (每题一行, 12 字段)
<ccl 编译命令行> × ccl数
<测试点数>
<输入文件> <答案文件> <分值>  × 测试点数
```

**题行 12 字段**：

| # | 语义 | 说明 |
|---|---|---|
| 1 | 题名 | 目录名/源文件基名/结果文件基名 |
| 2 | stdin 重定向 | 1=程序标准输入接数据 |
| 3 | stdout 重定向 | 1=程序输出写入文件 |
| 4 | 源码体积上限 KB | pickans 收卷过滤 |
| 5 | **输入文件名 = `<题名>.in`** | 配套 mv 机制（见下） |
| 6 | 选手输出文件名 | stdout 落盘名 = 比对器第 2 参 |
| 7 | 比对器名 | filter 目录中的可执行文件 |
| 8 | 选手可执行名 | execv 目标，漏填=程序不运行 |
| 9 | 时限（秒） | |
| 10 | 内存上限 MB | |
| 11 | 题型 | 0=源码提交 / 1=答案(.out)提交 |
| 12 | ccl 数 | 编译命令行数 |

**⚠ 字段 5 必须填 `<题名>.in`**：eval 每个测试点开始时 `mv <题名>.in → <题名>_std.in`
作 stdin、结束**恢复原名**（防复看）。填 `_std.in` 写法单点能过，但多点场景上一点
旧输入会覆盖新输入（实测踩坑）。

**示例**（两题：hello 单点 + plus 双点，g++）：

```
contest 0 2
hello 1 1 100 hello.in hello.out fulltext_e hello 1.0 0 0 1
cpp g++ -O2 -o %o %i
1
test1.in test1.ans 1.0
plus 1 1 100 plus.in plus.out fulltext_e plus 1.0 0 0 1
cpp g++ -O2 -o %o %i
2
plus1.in plus1.ans 1.0
plus2.in plus2.ans 1.0
```

配套数据文件（平铺 `/home/evaluate/`）：

```
test1.in = "hello world\n"    test1.ans = "hello world\n"
plus1.in = "1 2\n"            plus1.ans = "3\n"
plus2.in = "10 20\n"          plus2.ans = "30\n"
```

选手源码：`/home/<选手号>/<题名>/<题名>.cpp`。

比对器从 `assets/filter/` 选（复制到 `/home/evaluate/`）：`fulltext_e` 全文比对、
`slsn_int_e` 单行整数、`slmn_float_2e` 单行浮点 2 位、`mlmn_int_e` 多行整数、
`mlss_filterspace_e` 多行忽略空白、`network_e` 网络题特判……
分数 0–10 制写 `/tmp/_eval.score`，测试点得分 = filter分 × cfg分值 ÷ 10。

---

## 四、题目配置格式（bclient 自测用：selfeval.cfg）

bclient 的 `tasknames` 命令读取 `/home/evaluate/selfeval.cfg`（GUI 靠它显示题目与剩余次数）：

```
<最大自测次数> <题目数>
<题名1> <题名2> ... <题名N>
```

**示例**：

```
5 2
hello plus
```

`tasknames` 返回 `res=5, info="hello plus "`。

> bclient 的 `selfeval` 命令实际评测时仍调 `/usr/bin/eval` 读 **neweval.cfg**——
> 所以自测环境两份都要放，题名保持一致。

自测结果：`/home/self_evaluate/result/<时间戳>/<题>.rlt`（格式同下）。

---

## 五、.rlt 成绩单格式

```
{hello 100 Compile successfully
101 1.000000 0.003 Right Output!!!
}102 1.000000
```
- 首行 `{题名 编译码 文本}`：100 成功 / -100 无源码 / -101 编译失败
- 测试点行 `101 得分 用时秒 文本`：`Right Output!!!` / `Player's string differs with the answer` / `Time Limit Exceeded` / `Memory Limit Exceeded`
- 尾行 `}102 总分`

---

## 六、运行要求与已知边界

- Linux/WSL2 + Python3（纯标准库，无第三方依赖）；评测需 root（重建 /home/evaluater、setuid 60000 沙箱）
- `eval` 与比对器来自 `assets/`（复制到 `/usr/bin/eval` 与 `/home/evaluate/`）
- original FClient **目录**上传是原样透传（非递归）——试题与成绩单请逐文件分发
- `shutdown` 真的会关机；WebUI 已拦截高危命令，CLI 强制 yes 确认
- 官方对照：ISO 内 `/usr/local/arbiter/local/arbiter_local` 为同源引擎的 Qt GUI 版
  （CompileSrc/TaskTest/sampletest 等与 /usr/bin/eval 同名同签名），可作深挖参照
