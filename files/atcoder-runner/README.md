# AtCoder Runner

一个可执行入口 `atcoder`，运行于 Linux / WSL，主体只依赖 Python 3.8+ 标准库和已登录的 Codex CLI。按题并行解题、完成即提交，支持多个本地账户。运行时不依赖 Playwright。

`solver_prompt.md` 就是发给解题 Codex 的英文提示词，支持 `{{language}}`、`{{output}}`、`{{sample_validation}}`。默认启动参数使用 Full Access、本机执行、关闭审批与执行规则，通过 `web_search="disabled"` 关闭原生搜索；提示词同时禁止联网。外层程序不编译或检查解答。

## 使用

```bash
chmod +x atcoder cppobf
./atcoder login alice
./atcoder run abc344 alice
./atcoder do abc344_a alice
./atcoder do abc344_a alice gpt-6-astra/xhigh
./atcoder run abc344 alice --urgent
./atcoder do abc344_a alice --no-urgent
./atcoder run --help
```

`alice` 是自定义的本地账户别名，`run` / `do` 的 `user` 填这个别名。`login` 保存浏览器 Cookie 中 **REVEL_SESSION 的值**，省略时隐藏输入；也支持 `./atcoder login alice 'COOKIE值'`。这是唯一需要用户提供的 AtCoder 凭据，保存在主目录 `config.json`。程序写入配置时申请文件权限 600；WSL 挂载的 Windows 文件系统还受 Windows 权限控制。服务器更新的 Cookie 仅保存在对应账户的内存中，各账户隔离。

`run` 以单比赛、单账户运行。赛前获取起止时间和首页配点表的题目标号，只取标号、不取分数，保存到比赛目录的 `contest.json`：`start`、`end`、`labels`、`count` 和公开后的官方题号 `tasks`。完整配置直接复用，尚无题目标号时再次获取。到开赛时间立即获取正式题号，所有题并行获取 `statement.md`、调用 Codex、混淆和提交。题面包含完整样例。

题目分数从题目详细页写入 `statement.md`，解题前再读取，支持 `points`、`point`、`pts`、`pt`、`点`。按分数规则选择模型、深度及样例验证选项；未知分数使用 `default`。比赛配置不保存分数或模型。

`run` 生成 `sol.cpp`、`sol.py` 或 `sol.js`。`do` 接收官方题号，例如 `abc344_a`，没有代码时生成下一个编号文件 `sol1.cpp` / `sol1.py` / `sol1.js`。已有非空代码时直接复用：`do` 优先最新编号，其次无编号；`run` 优先无编号，其次最新编号。只复用当前语言的文件。指定模型不会强制重做已有代码。`do` 不监听判题或自动重做；它只缓存比赛起止时间，不建立完整题号列表。

## 提交准备和迫切模式

准备阶段读取一次 [ABC001 提交表单](https://atcoder.jp/contests/abc001/submit)，从真实表单选择配置语言的编号，并在该账户、本次运行内共享 CSRF 字段。读取与比赛信息准备并行，赛前尽量完成，不因尚未完成的表单请求延后开赛抓题。各题代码完成后直接使用预取字段向目标比赛 POST，不再逐题读取表单。普通网络重试复用字段；表单读取失败或服务器明确拒绝 CSRF 时重新获取。字段不写入磁盘，不在账户之间共享。

`urgent` 默认 `false`：等待 Codex 正常结束，让它完成测试和修正，然后提交，没有固定的一分钟等待。

`urgent: true` 或 `--urgent` 开启迫切模式：约每 20 毫秒检查目标代码文件，发现非空 UTF-8 内容就截取该版本，立即进入混淆和提交流程。**提交期间 Codex 继续运行；先完成提交流程，再关闭该轮 Codex，不等待它停止才发起提交。**这一轮提交和复制固定使用截取版本，后续改写不影响正在发送的代码；关闭后将原始代码文件恢复为该版本，供下次复用。失败或手动退出也会清理子进程。

迫切模式不等待文件稳定或样例验证完成；即使提示词要求验证样例，也可能在验证前提交。`--no-urgent` 可临时关闭配置中的迫切模式。这两个命令行开关不改写配置文件。

## 配置

所有配置相对于可执行文件所在的主目录读取，无须 `--config` 参数。只支持本页描述的当前格式，不迁移旧配置。账户别名由 `login` 保存到同一文件的 `accounts`，不要把账户 Cookie 或 API Key 放入发布包。

### 语言及模型

```json
{
  "language": "cpp",
  "urgent": false,
  "default": {"model": "gpt-6-luna", "level": "max", "verify_samples": true},
  "rules": [
    {"min": 0, "max": 300, "model": "gpt-6-luna", "level": "medium", "verify_samples": false},
    {"min": 300, "max": 600, "model": "gpt-6-luna", "level": "high"},
    {"min": 600, "max": null, "model": "gpt-6-luna", "level": "xhigh"}
  ]
}
```

| `language` | 生成文件 | AI 使用的语言 | AtCoder 提交语言 |
| --- | --- | --- | --- |
| `cpp`（也可写 `C++`） | `sol.cpp` | C++17 | C++23 |
| `python`（也可写 `Python 3` / `py`） | `sol.py` | Python 3 | Python 3 / CPython 3 |
| `javascript`（也可写 `JS`） | `sol.js` | 兼容 Bun 的 JavaScript | JavaScript (Bun) |

不会将 Python 选择为 PyPy，也不会将 JavaScript 选择为 Node.js。语言编号从预取表单读取；没有匹配项时直接报错。

`default` 用于未知分数和所有重新解题。`rules` 按顺序匹配，区间包含 `min`、不包含 `max`；`max: null` 表示没有上限。`verify_samples` 必须是布尔值：规则省略时继承 `default`，两处都省略时为 `true`。为 `true` 时提示词要求实际运行全部样例并修正失败；C++ 使用 `g++ -std=c++17`，Python 使用 Python 3，JavaScript 使用 Bun，需自行准备对应运行环境。为 `false` 时明确跳过样例测试。修改规则不用删除比赛配置。

### Codex 启动参数

`codex` 是 CLI 路径或 PATH 中的命令名。`codex_args` 是完整启动参数数组，**必须包含 `exec` 子命令**，省略时使用下列默认值。提供数组时整体替换默认值，请保留所需的权限及禁用联网参数。

```json
{
  "codex": "codex",
  "codex_args": [
    "--no-daemon", "--ask-for-approval", "never",
    "exec", "--ignore-user-config", "--ignore-rules", "--ephemeral",
    "--skip-git-repo-check", "--dangerously-bypass-approvals-and-sandbox",
    "-c", "web_search=\"disabled\"",
    "-c", "features.plugins=false",
    "-c", "features.apps=false",
    "-c", "features.multi_agent=false",
    "-c", "features.browser_use=false",
    "-c", "features.computer_use=false",
    "-c", "features.skill_search=false"
  ],
  "http_timeout": 30,
  "ai_timeout": 1800
}
```

程序自动在这些参数之后追加工作目录 `--cd`、选定的 `--model`、思考深度配置、题目图片和从标准输入读取提示词的 `-`；不要在数组中重复设置。参数内可用 `${root}` 指代主目录、`${problem}` 指代题目目录。不经过 shell 展开；参数必须适用于本机安装的 Codex 版本。

### 每种语言的混淆器

```json
{
  "obfuscators": {
    "cpp": {
      "command": ["./cppobf", "--compress", "--flatten", "--opaque", "${file}", "-o", "${output}", "--", "-std=c++17"],
      "timeout": 30,
      "env": {}
    },
    "python": {
      "command": ["python3", "-m", "PyObfuscator", "${file}", "-o", "${output}", "-l", "6"],
      "timeout": 30,
      "env": {}
    },
    "javascript": {
      "command": ["npx", "--no-install", "javascript-obfuscator", "${file}", "--output", "${output}", "--target", "node", "--compact", "true", "--vm-obfuscation", "true", "--pro-api-token", "${env:JAVASCRIPT_OBFUSCATOR_API_KEY}"],
      "timeout": 300,
      "env": {}
    }
  }
}
```

只调用当前语言的配置。三种语言统一使用输入文件和输出文件。`command` 是程序及参数的字符串数组，不经过 shell；非空命令必须同时含 `${file}` 和 `${output}`，缺少任一项直接报错。空数组 `[]`、空值或省略该语言的命令表示不混淆。`timeout` 单位为秒，省略时为 30；`env` 叠加到当前进程环境，值必须是字符串。执行工作目录为主目录，相对可执行路径也以主目录为准。

占位符：

- `${file}`：本轮原始代码快照的绝对路径，位于临时目录，不是 Codex 正在改写的文件。
- `${root}`：项目主目录。
- `${output}`：必需的临时输出文件路径，工具必须将完整混淆代码写到这里。
- `${env:NAME}`：从本次混淆进程的环境中读取 `NAME`，包括配置的 `env`；变量未设置或为空时报错。

例如 `"command": ["my-obfuscator", "${file}", "-o", "${output}"]`。stdout 始终忽略，即使输出了完整代码也不会作为提交内容；输出文件缺失或为空就报错。控制台 banner 和进度不影响结果。

每一轮生成或复用代码都会重新混淆；网络重试使用同一份内存结果。提交与复制完全一致，所有临时源码和混淆结果在读取后删除，不保留或复用 `sol.obf.cpp` 等文件。题目目录中的旧 `sol*.obf.cpp` / `.py` / `.js` 会清理，原始解答保留。工具缺失、超时、返回错误或输出为空时直接报错，不自动安装、不回退提交原始代码。

随包的 `cppobf` 用于 Linux / WSL x86_64，依赖 LLVM/Clang 18 动态库。所有混淆器均只使用 `obfuscators` 中的命令、超时和环境配置，没有语言专用的回退分支。

#### Python：PyObfuscator

假设用户已在命令所指的 Python 环境中安装 [PyObfuscator PyPI 包](https://pypi.org/project/PyObfuscator/)。可先手动检查：

```bash
python3 -m PyObfuscator --help
```

包要求 Python 3.9+；主体脚本的 Python 3.8+ 要求与混淆器的解释器要求独立。可将 `command` 首项改成已安装该包的 Python 绝对路径。建议与提交使用的 Python 版本保持兼容。默认命令用 `-o ${output}` 读取纯代码，避免将工具控制台信息提交。未安装时程序报错，请自行准备环境或把该语言的 `command` 设为 `[]`。

#### JavaScript：javascript-obfuscator

在项目主目录自行安装 [javascript-obfuscator](https://github.com/javascript-obfuscator/javascript-obfuscator)。默认配置保留 Pro VM 混淆，需要所安装版本支持 `--pro-api-token` / `--vm-obfuscation`，并在 [obfuscator.io](https://obfuscator.io/) 准备 API Key：

```bash
npm install --prefix . javascript-obfuscator
```

直接使用官方 CLI，不提供 wrapper。`npx --no-install` 只调用已安装的程序，不在比赛中下载依赖。配置 Key：

```json
"javascript": {
  "command": ["npx", "--no-install", "javascript-obfuscator", "${file}", "--output", "${output}", "--target", "node", "--compact", "true", "--vm-obfuscation", "true", "--pro-api-token", "${env:JAVASCRIPT_OBFUSCATOR_API_KEY}"],
  "timeout": 300,
  "env": {"JAVASCRIPT_OBFUSCATOR_API_KEY": "YOUR_API_KEY"}
}
```

也可在终端环境设置同名变量，保持配置中的 `env: {}`。CLI 的参数直接写在 `command` 数组中，输入使用 `${file}`，输出使用 `--output ${output}`，其余参数自行调整。只使用本地免费混淆时，可改为以下命令，不需要 API Key：

```json
"command": ["npx", "--no-install", "javascript-obfuscator", "${file}", "--output", "${output}", "--target", "node", "--compact", "true"]
```

Pro 混淆会请求其服务，与解题 Codex 的禁止联网要求分开；`proxy` 字段只控制 AtCoder 请求，不控制这个外部程序。程序不会安装 npm 包、申请 Key 或自动配置 API。缺少依赖或 Key 时直接报错；`command: []` 可关闭混淆。

### AtCoder 代理

`"proxy": "socks5://127.0.0.1:1080"`，也支持 `socks5h://` 和 `http://`。SOCKS5 两种写法都由代理解析域名，支持 URL 编码的用户名和密码。比赛信息、题面、图片、表单、提交及判题监听全部使用该代理，并验证 HTTPS 证书。空字符串、`null` 或省略时直连，不读取终端代理环境变量。

## 判题与退出

`run` 使用一个统一的 My Submissions 监听任务，每十秒读取一次，必要时翻页。右侧显示 WJ、WR、判题进度和最终结果；只按本次提交 ID 判断，历史 AC 不算本次通过。旋转动画表示 AI 解题，向上箭头表示提交。

WJ、WR 和测试进度继续等待。最终非 AC 时删除本轮被判错的源码，使用 `default` 的模型、深度和样例选项，开启全新 Codex 会话，重新生成当前语言的 `sol.*` 并提交。首次之后最多重做 **三次**，即最多 **四轮解题提交**。网络重试和监听错误不消耗解题次数。

全部 AC 后自动退出。某题重做耗尽或普通错误最终失败时，其他题继续，全部处理完后退出，返回 1。全部通过返回 0，手动退出返回 130。CF 受阻时保留界面，必须手动退出。

Summary 显示通过题数 / 总题数、会话时长、总罚时。这里“总罚时”按项目约定为 **各题最后一轮全部提交完成的时间 − 开赛时间**，优先用服务器提交时间，不包含等待判题，不另外累加官方 WA 罚时。有题未成功提交则显示“未完成”。

界面显示北京时间、会话用时、开赛倒计时、比赛进行时间 / 总时长。默认鼠标滚轮滚动，点击复制按钮或顶部退出按钮；方向键、数字键和 `q` 可备用。复制在后台执行。

## 文件与账户共享

```text
项目目录/
  atcoder
  config.json
  solver_prompt.md
  README.md
  cppobf
  node_modules/                 # 使用 JavaScript 混淆时自行安装
  abc344/
    contest.json
    .lock
    A/
      .lock
      statement.md
      sol.cpp                  # 按语言可为 sol.py / sol.js
      sol1.cpp                 # do 生成的编号版本
    B/...
```

题目锁保护获取、生成、混淆及提交；迫切模式在提交后关闭 Codex 再释放锁，等待判题时不持锁。不同账户共享题面与代码，每个账户独立提交、读取结果。`.lock` 空文件保留，操作系统随进程退出释放锁。题目目录不保存 AI 日志、拆分样例、报告或 Solution 文档；题目图片临时下载供 Codex 使用，完成后删除。

## 网络错误与 CF

获取比赛信息、题面和图片遇到普通错误时，等待 **五秒**重试，最多 **十次重试**，共最多 11 次请求。赛前未公开的 `/tasks` 只尝试一次，开赛后再获取正式题号。

提交的普通网络错误或 HTTP 429 同样五秒后重试，最多十次，正常使用准备阶段缓存的表单。POST 的合法成功重定向立即代表服务器接受了代码，不再跟随读取记录页。连接中断导致提交结果不确定时，先核对新增记录及源码；确认已接受则进入判题监听，确认没有提交才重新 POST，无法确认时也不会重复 POST。

**提交表单、提交或判题相关页面遇到 CF 时，零重试，显示复制按钮并保留界面，直到手动退出；其他题继续。**提交页跳转登录也进入复制流程，不直接宣称 REVEL_SESSION 无效。程序不处理验证码。

点击复制得到完整提交代码，启用混淆时为内存中的混淆结果。支持 `clip.exe`、`wl-copy`、`pbcopy`、`xclip` 或 OSC 52。非交互模式无法点击，CF 受阻后打印提示并等待手动终止；需要复制按钮时请直接在交互式终端运行，不要重定向输出。

## 验证与发布

开发测试：

```bash
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest discover -s tests -v
```

93 项测试通过，包含并行处理、重试、判题与重做、三语言表单选择及输出、启动参数、统一文件混淆及临时文件清理、迫切模式下提交期间 Codex 仍运行以及提交后再关闭。JS 使用模拟 npx 可执行文件验证参数、输入输出路径及 API Key 传递，没有请求收费 API；未安装 PyObfuscator 时的报错已验证，没有自动安装依赖。另外实际运行了随包 cppobf，编译并执行混淆结果。

此前已实际读取 awc0177 五题，确认分数 200、233、300、400、466 及规则深度；已验证 abc480 赛前七题。此前读取真实 ABC001 提交表单被登录跳转阻挡，语言选择使用真实结构的模拟表单验证，未发送真实测试提交，也未完成真实提交判题联测。

`outputs/atcoder-runner.zip` 包含上述五个核心文件，不含账户凭据、API Key、代理凭据、比赛数据、node_modules 或开发测试。现有本机账户和模型配置保留。
