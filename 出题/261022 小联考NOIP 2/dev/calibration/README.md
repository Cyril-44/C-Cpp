# 窗口校准：出题报告

本题使用一次差分方案，保留本目录的 WBLT、独立 AVL、生成器和核心验题代码。

本联考版将上界从 $3\times10^6$ 进一步缩至 **$n,m\le10^5$**，规模档为 1000、10000、100000；限制为 **1 秒、512 MiB、64 位 GNU C++17**。题意、模数和 20/15/15/20/30 分的分组定义保持一致。

## 文件安排

- `statement.md`：最终题面；`sol/solution.md`：题解、复杂度与实现注意事项。
- `sol/std_wblt.cpp`：一次差分、两矩、共享倍增；沿用 `!Luogu/P8524.cpp` 的 `clone/cut/split/merge` 写法，将校准、仿射、查询分别封装。
- `sol/std_avl.cpp`：独立满分参考，直接维护原数组和仿射标记的共享 AVL，不依赖差分边界公式。
- `sol/partial_brute.cpp`、`sol/partial_special.cpp`：直接模拟，以及等差赋值/仿射线段树加一般校准模拟。
- `gen/gen.cpp`、`gen/val.cpp`：统一使用 Git 根目录的 `testlib.h`；`gen/cases.tsv` 逐点列出固定种子、规模、策略及用途。
- `gen/genall.py`：一键生成、校验、交叉核对并发布全部数据；`review/`：独立 oracle、结构审计、工具检查、部分分评分、性能脚本及独立样例文件。

## 数据与评分

| 组 | 分值 | 点数 | 条件（$N=\max(n,m)$） |
| --- | ---: | ---: | --- |
| 1 | 20 | 8 | $N\le1000$ |
| 2 | 15 | 6 | $N>1000$，没有操作一 |
| 3 | 15 | 7 | $N>1000$，有操作一且全部 $k=1$ |
| 4 | 20 | 8 | $1000<N\le10000$，有 $k>1$ 的操作一 |
| 5 | 30 | 11 | $10000<N\le100000$，有 $k>1$ 的操作一 |

各组内全部通过才得分，组间分数相加。当前 40 个正式点共有 1,878,329 次操作、355,942 次查询，输入 44,843,018 字节、答案 2,960,808 字节。3 个正式点和 1 个大样例达到 $n=m=100000$。固定构造参数和用途见 `gen/cases.tsv`。

构造覆盖 $n=1$、长度 2/3、$k=r-l$ 恒等、只改一项的 $k=r-l-1$、$r=n$ 与 $r=n-1$、单点仿射、零/单位/取负仿射、标记复合、素数及 2 幂附近长度、小周期、大周期、非整周期尾部、不同偏移、嵌套共享与回收。`preserved_seed` 长期保留前半区的大随机种子，专门防止小周期覆盖迅速抹平输入结构。

生成脚本已在当前 100000 上限下重新生成并校验全部 40 个正式点、3 个题面原样例和 3 个大样例；每点均通过 validator，并由 WBLT 与独立 AVL 交叉核对，第 1 组另与暴力比较。历史 500000 上限、4 秒限制下的部分分得分不代表当前版本；本版 1 秒得分需用 `review/score_partial.py` 重新测定。

## 一键生成

从任意工作目录均可运行；省略参数时输出至联考目录的 `data/calibration` 与 `down/calibration`：

```bash
# 在 dev/calibration 下
python3 gen/genall.py

# 在任意工作目录下
python3 /absolute/path/dev/calibration/gen/genall.py
python3 /absolute/path/dev/calibration/gen/genall.py '/custom/data dir' '/custom/down dir'
```

相对自定义路径按调用时的当前目录解释。脚本自行定位 Git 根目录、编译构造器、testlib 校验器及三个核对程序；需要 `g++`、`python3` 和 Git。生成和发布代码均位于 `gen/`。每点先校验输入分组，再以各自 4 秒的生成核对超时运行 WBLT 与独立 AVL（该超时不改变题面的 1 秒限制），检查答案数量、规范模数剩余类及逐项一致；第 1 组和题面样例另与暴力程序比较。

全部检查成功后才复制声明的 46 对文件：40 个正式点按 `gen/cases.tsv` 命名；下发文件 `calibration1.in/ans` 至 `calibration3.in/ans` 按题面顺序直接提取原输入与答案，`calibration4.in/ans` 至 `calibration6.in/ans` 为大样例。原样例答案只作交叉核验，发布时保留题面原文。所有下发文件权限设为 `664`，不额外下发 manifest。只覆盖这批明确命名的文件，不删除目标目录中的其他文件；某点检查失败时原发布数据保持原状。临时构建目录由脚本创建并在退出时自行清理。默认只向终端输出进度；需要逐点耗时、大小和哈希时，可显式加 `--report .build/generation.json`。

最近一次正式数据验收已从 `/tmp` 执行默认路径全量生成，并在含空格的自定义相对目录复现 46 对文件，92 个文件哈希全部一致，保留了 4 个无关哨兵文件。

正式数据和大样例已按当前 100000 上限生成并通过双解核对；生成器、校验器和清单中的构造参数保持原样。

## 正确性与工具验收

2026-10-23 重写标准解：保留原 WBLT 的拆分、拼接和回收接口，使用“拆段—修改—拼回”组织三类操作。`clone` 会把新根引用数重置为 1；查询只读两个矩，不产生新节点。结构审计集中在 `review/wblt_audit.hpp`，标准解不再包含审计、性能统计或批量编辑框架。

新实现已核对全部 40 个正式点与 6 个下发样例，答案与已发布数据全部相同；独立题意 oracle、WBLT 和独立 AVL 在 1213 例（1200 个随机实例、3 个样例、10 个固定实例）上得到相同的 181,291 个答案，共执行 239,294 次操作。共享结构随机压力通过 100,000 次操作；穷举至 14 个叶子的全部 1558 种严格三倍平衡形状，通过 21,483 次分裂重拼、5047 次拼接和 2748 次共享重复，引用计数、平衡、两个矩及回收断言均成立。ASan/UBSan 另通过 213 例、41,104 次题目操作及 15,000 次结构随机操作，无错误报告。

以下保留此前实现与数据工具的历史验题结论；其中 `AUDIT`、旧结构穷举数量和旧耗时属于此前实现，新标准解以上面的验收为准。

清理后的快速冒烟通过：3 个独立样例、10 个固定边界和 20 个随机实例，共 33 例、8049 次操作、6507 个答案，WBLT、AVL、暴力及专用部分分四路一致；84 项工具检查、100 次共享树随机操作及正式点 `01_01` 的两份部分分程序也均通过。

Python oracle 直接从题目窗口和递推期望答案；WBLT 开启 `AUDIT`，独立 AVL 开启 `AVL_AUDIT`，与 C++ 暴力、专用部分分四路核对。全部 **1210 例、239,264 次操作、181,275 个答案**一致，包含 1200 个随机实例和 10 个固定边界实例。另完成 31,302 次校准公式穷举、41,823 次仿射公式穷举、5000 次随机公式检查。

共享结构通过 100,000 次随机操作压力；穷举至 14 个逻辑叶子的 11,110 种平衡形状，覆盖 156,955 次分裂重拼、29,983 次拼接、15,900 次共享重复。所有结构审计、引用计数、平衡和矩断言通过。

testlib 正反例与生成策略共有 84 项，其中包含 59 种生成参数组合，错误分组、超界 $n,m$、非法窗口、缺少查询等全部按预期拒绝；输出数量、模数范围及答案对比有 8 组正反例。每个正式输入已使用 `--group` 校验；分组采用 `validator.group()` 读取 testlib 原生选项。专用部分分另通过 600 个随机实例和 3 个公开样例。AddressSanitizer、UndefinedBehaviorSanitizer 与 WBLT 结构审计还通过 210 例、41,074 次操作、31,215 个答案，未报告内存或未定义行为错误。


## 核心验题入口

`review/` 保留以下 8 份核心源码及 `review/samples/` 中的 3 对独立样例文件。历史 JSON、日志和重复清单已清理，重要验收结论保留在本报告中。

| 文件 | 用途 |
| --- | --- |
| `review/oracle_fuzz.py` | 按题意直接计算窗口和，核对题面样例、固定边界及随机实例；支持多份候选程序、公式穷举和失败重放，也覆盖部分分程序 |
| `review/wblt_exhaustive.cpp` | 穷举平衡形状，检查分裂、拼接和共享重复 |
| `review/wblt_audit.hpp` | 供两个结构测试共用，检查共享图、引用次数、严格三倍平衡、矩及回收槽位 |
| `review/wblt_stress.cpp` | 对共享树随机操作并检查引用计数、平衡与矩 |
| `review/check_output.py` | 统一检查答案数量、规范模数剩余类与参考答案 |
| `review/tool_tests.py` | 校验器正反例、生成策略与输出检查器验收 |
| `review/score_partial.py` | 按正式数据及组内全过规则评分，默认 1 秒、512 MiB；`--case` 可仅冒烟指定点 |
| `review/benchmark_scale.sh` | 自动编译并测量满规模性能，临时结果退出时清理 |

在本题目录下，先构建到被忽略的 `.build/`，再选择所需测试：

```bash
mkdir -p .build
python3 -c 'from pathlib import Path; from gen.genall import compile_tools; compile_tools(Path(".build").resolve())'
g++ -std=c++17 -O2 sol/partial_special.cpp -o .build/partial
python3 review/oracle_fuzz.py --std .build/std --reference .build/avl --reference .build/partial --brute .build/brute --cases 20 --self-check 0
python3 review/tool_tests.py
python3 review/score_partial.py --case 01_01

g++ -std=c++17 -O2 review/wblt_stress.cpp -o .build/wblt_stress
.build/wblt_stress 20261022 2 50
g++ -std=c++17 -O2 review/wblt_exhaustive.cpp -o .build/wblt_exhaustive
.build/wblt_exhaustive
```

oracle 默认读取 `review/samples/` 的独立样例文件，并将失败实例写入 `.build/oracle_failures/`；仅显式指定 `--report` 时保存报告。工具验收、评分和性能结果默认输出至终端，可自行重定向到 `.build/`。两个结构测试自带共享图审计，编译时须保留断言；公式完整穷举可运行 `python3 review/oracle_fuzz.py --self-check-only`。

## 性能与限制选择

测试环境：WSL2 Linux x86-64，g++ (Ubuntu 10.5.0) 10.5.0，`-std=c++17 -O2 -DNDEBUG`。下表为 2026-10-23 重写后的 WBLT 本机实测；正式数据及下发样例逐项对照已发布答案，7 种满规模策略与独立 AVL 交叉核对。

| 测试范围 | 新 WBLT 最慢 |
| --- | ---: |
| 40 个正式点 | 0.39 秒 |
| 6 个下发样例 | 0.45 秒 |
| 7 种策略，各 $n=m=100000$ | 0.37 秒 |

上述测试峰值内存为 10,236 KiB。`review/benchmark_scale.sh` 可以重新编译并测试两份标准解。当前题面限制为 1 秒；目标 OJ 的 CPU 与编译器可能不同，WSL 耗时不保证其他机器性能。
