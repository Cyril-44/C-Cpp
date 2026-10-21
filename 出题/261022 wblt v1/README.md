# 窗口校准

一道区间序列维护题。输入给出模 $998244353$ 的序列，要求支持窗口校准、区间仿射变换与区间求和。正式题面只描述窗口条件；二阶差分、共享结构等解法信息放在题解中。

全局限制为 $1\le n,m\le3000000$，时间限制 **30 秒**、空间限制 **512 MiB**，至少有一次查询。输入顺序是 `n`、`n` 个初值、`m`、`m` 次操作。

## 文件说明

| 路径 | 用途 |
| :-- | :-- |
| `report.md` | 完整出题报告、逐点实测、实际部分分及验题结论 |
| `statement.md` | 完整中文题面、三组样例及分组表 |
| `editorial.md` | 数学推导、满分结构、复杂度、部分分与替代解法 |
| `solutions/std.cpp` | C++17 标准程序；共享权重平衡树与引用计数回收 |
| `solutions/reference_avl.cpp` | 直接维护原数组的独立 AVL 满分参考，用于全量交叉核对 |
| `solutions/brute.cpp` | 直接递推窗口和的独立暴力程序 |
| `solutions/partial_special.cpp` | 独立维护原序列的线段树，前 3 组基本覆盖，当前正式数据实测 70 分 |
| `samples/` | 三组 `.in` / `.out` 及覆盖说明 |
| `config/judge.json` | 30 秒、512 MiB、checker 约定与正式组内文件映射 |
| `config/scoring.json` | 全局限制及互斥分组的机器可读配置 |
| `config/data_spec.json` | 每个数据点的规模、种子、生成模式与用途 |
| `tools/generate.py` | 按固定种子生成测试数据及清单 |
| `tools/validator.cpp` | 严格校验输入合法性与所属分组 |
| `tools/checker.py` | 按整数 token 比较输出，并检查规范模数范围 |
| `tools/build.py` | 统一构建标准、暴力、部分分、校验器与调试程序 |
| `tools/oracle_fuzz.py` | 数学自检、独立 oracle 对拍及失败输入保存 |
| `tools/test_partial.py` | 独立部分分程序的可复现随机对拍 |
| `tools/wblt_stress.cpp` | 直接测试共享树的分裂、拼接、覆盖、懒标记与引用计数 |
| `tools/check_integrity.py` | 检查发布文件大小与 SHA-256；轻量包使用 `--skip-data` |
| `tools/score_solution.py` | 按正式分组测量实际得分 |
| `tools/runner.py` | 带超时、内存采样和输出记录的运行器 |
| `tools/verify_package.py` | 生成答案、验证全包、交叉核对与逐点实测 |
| `audit/` | 数学审查、数据设计、随机对拍和实际验收记录 |
| `data/` | 生成的正式输入、答案及生成清单 |

需要 C++17 编译器和 Python 3；Python 工具仅使用标准库。以下命令在本目录执行。示例可执行文件采用 Windows 的 `.exe` 后缀，在 Linux 下去掉后缀即可。

## 构建与样例

```text
python tools/build.py --out build --audit
```

默认编译器为 `g++`，可用 `--cxx clang++` 或环境变量 `CXX` 指定。常规构建会产生 `std`、`reference_avl`、`brute`、`validator`、`partial_special`。加上 `--audit` 还会构建 `std_audit`、`reference_avl_audit`、`wblt_stress`、`wblt_exhaustive`。

`std` 使用 `-O2 -DNDEBUG -DPROFILE`；统计信息写入标准错误，不影响答案。`std_audit` 在每次操作后检查逻辑平衡、矩、物理引用计数与空闲链表，仅用于小规模验题。

```text
python tools/runner.py --exe build/std.exe --input samples/01.in --output build/01.out
python tools/checker.py samples/01.in build/01.out samples/01.out
```

第二、三组样例同理。`checker.py` 的三个位置参数依次为输入文件、待测输出、标准答案。答案文件不能含非整数 token、缺少或多出答案，输出整数必须在 $[0,998244352]$ 内。

## 数据生成与输入校验

先查看数据点，而不产生输入：

```text
python tools/generate.py --list
python tools/generate.py --group 4 --json
```

生成单点、一个组或全部数据：

```text
python tools/generate.py --case g1_sweep --output build/g1_sweep.in
python tools/generate.py --group 1 --output-dir build/group1
python tools/generate.py --all --output-dir data
```

每个数据点的随机种子固定。数组与操作使用独立随机流，生成结果可复现。批量输出会产生 `generation_manifest.json`，记录实际规模、分组和输入文件名。生成器的 `--max` 可在本地试验时替换总规模；正式数据使用配置中的 $3000000$。

单点校验可通过 Python 传入标准输入，避免不同命令行对输入重定向的差异：

```text
python -c "import subprocess; subprocess.run(['build/validator.exe','--group','1'], stdin=open('data/g1_min.in','rb'), check=True)"
```

省略 `--group` 时只要求输入合法，并报告实际所属组。校验器支持 `--max`、`--max-n`、`--max-m`、`--small-max`、`--medium-max`，便于同步检查本地缩放数据。

## 分组计分

采用组内全部通过才得分、各组分数相加的计分方式。记 $N=\max(n,m)$，五组互斥且覆盖全部合法输入：

| 组 | 分值 | 限制 | 对应做法 |
| :--: | :--: | :-- | :-- |
| 1 | 20 | $N\le2000$ | 直接维护原序列，线性执行每次操作 |
| 2 | 15 | $N>2000$，没有校准操作 | 区间仿射与求和线段树 |
| 3 | 15 | $N>2000$，有校准且所有校准的 $k=1$ | 二次多项式赋值、仿射与求和线段树 |
| 4 | 20 | $2000<N\le20000$，存在 $k>1$ 的校准 | 一般操作中档；允许额外空间的共享结构等 |
| 5 | 30 | $N>20000$，存在 $k>1$ 的校准 | 可回收共享结构或其他满足限制的正确算法 |

`partial_special.cpp` 对 $k=1$ 使用多项式赋值，对一般 $k$ 使用窗口递推模拟，因此覆盖第 1、2、3 组共 50 分；在当前正式数据和 30 秒限制下，第 4 组也通过，实测 70 分，第 5 组超时。C++ 直接暴力实测通过第 1、4 组，共 40 分。原始记录见 `audit/score_partial.json` 和 `audit/score_brute.json`。

## 独立 oracle 与结构测试

```text
python tools/oracle_fuzz.py --self-check-only --self-check 10000
python tools/oracle_fuzz.py --std build/std_audit.exe --brute build/brute.exe --cases 3000 --n-max 80 --m-max 240
python tools/test_partial.py --exe build/partial_special.exe
build/wblt_stress.exe 20261021 100 1000
```

oracle 使用原序列及窗口和递推，不调用共享树。对拍器支持指定种子、单个输入重放、超时与失败目录，完整参数见 `--help`。`test_partial.py` 默认运行 3 个样例和 600 个随机实例，一半允许任意 $k$，一半限定 $k=1$，报告写入 `audit/partial_fuzz.json`。

结构测试的三个可选参数依次为种子、轮数、每轮操作数。它直接对共享树进行区间复制、乘法和重复位置加法，逐步检查引用计数、逻辑平衡、矩和所有元素。`std_audit` 与结构测试的断言检查不可使用 `-DNDEBUG` 关闭。

`tools/build.py --no-gc` 另行构建 `std_no_gc`，用于检验不回收版本的空间行为；该版本是验题用对照程序。

## 全包验收与答案生成

在生成了完整 `data/generation_manifest.json` 后执行：

```text
python tools/verify_package.py --std build/std.exe --reference build/reference_avl.exe --validator build/validator.exe --brute build/brute.exe --partial build/partial_special.exe --data-dir data --scratch build/verify --write-answers
```

该命令逐点校验分组，以独立 AVL 参考核对所有答案，核对通过后发布 LF 换行的 `.ans`；在第 1 组另用暴力核对，在前 3 组另用独立部分分程序核对，并将哈希、答案数量、耗时、内存和节点池统计写入 `audit/full_verification.json`。已有答案时去掉 `--write-answers` 即可复验。`--write-answers` 必须同时传入 `--reference`，避免仅由同一程序生成和自验答案。

使用 `--case ID` 可只验收某一点，可多次指定。`--timeout`、`--memory-mb` 控制本次本地运行的资源阈值；这些参数是验题器选项，不能替代正式评测时限配置。

标准算法的理论总时间为 $O(n+m\log n)$，空间为 $O(n)$；回收大段旧节点的代价按总分配量摊还。40 个正式点在 Windows 与 Linux/WSL2 上全部通过，两个环境都与独立 AVL 逐项核对。WBLT 最大耗时分别为 10.829 秒和 10.401 秒，最大测得内存分别约 200.3 MiB、198.0 MiB。正式配置为 30 秒、512 MiB。完整方法、采样限制、逐点记录和未进行真人试做的说明见 `report.md`，原始记录保留于 `audit/`。

## 发布完整性

完整包已含 40 组正式输入和答案。解压后先运行 `python tools/check_integrity.py`；轻量源代码包运行 `python tools/check_integrity.py --skip-data`。`manifest.json` 记录每个交付文件的大小与 SHA-256，`checksums.sha256` 可供其他工具使用。重跑验题会更新审计记录，旧发布清单自然不再覆盖这些本地改动；这不等于计算结果错误。
