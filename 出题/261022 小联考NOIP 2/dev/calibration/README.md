# 窗口校准：出题报告

本题使用 Codex 工作目录 `D:/Hardsoft/Codex/2026-10-21/p8524-ynoi2078-a-theory-of-consciousness/outputs/window-calibration-linear` 的一次差分版本。保留并整理本目录已有的 WBLT、独立 AVL、生成器和审查代码；未采用其他目录的旧题版本。

本联考版将上界从 $3\times10^6$ 缩至 **$n,m\le5\times10^5$**，限制定为 **4 秒、512 MiB、64 位 GNU C++17**。题意、模数和 20/15/15/20/30 分的分组定义保持一致。以下记录全部来自本次 4 秒版重新验收，原 30 秒版的性能、部分分和数据统计不作为本版结论。

## 文件安排

- `statement.md`：最终题面；`sol/solution.md`：题解、复杂度与实现注意事项。
- `sol/std_wblt.cpp`：一次差分、两矩、权重平衡拼接树、共享倍增和引用计数回收。
- `sol/std_avl.cpp`：独立满分参考，直接维护原数组和仿射标记的共享 AVL，不依赖差分边界公式。
- `sol/partial_brute.cpp`、`sol/partial_special.cpp`：直接模拟，以及等差赋值/仿射线段树加一般校准模拟。
- `gen/gen.cpp`、`gen/val.cpp`：统一使用 Git 根目录的 `testlib.h`；`gen/cases.tsv` 逐点列出固定种子、规模、策略及用途。
- `gen/genall.sh`：一键生成、校验、交叉核对并发布全部数据；`review/`：对拍、样例、工具检查、性能与验收记录。

## 数据与评分

| 组 | 分值 | 点数 | 条件（$N=\max(n,m)$） |
| --- | ---: | ---: | --- |
| 1 | 20 | 8 | $N\le2000$ |
| 2 | 15 | 6 | $N>2000$，没有操作一 |
| 3 | 15 | 7 | $N>2000$，有操作一且全部 $k=1$ |
| 4 | 20 | 8 | $2000<N\le20000$，有 $k>1$ 的操作一 |
| 5 | 30 | 11 | $N>20000$，有 $k>1$ 的操作一 |

各组内全部通过才得分，组间分数相加。40 个正式点共有 7,456,329 次操作、1,262,062 次查询，输入 189,948,290 字节、答案 10,645,405 字节。两个正式点同时达到 $n=m=500000$，另有 1 个同规模的保留种子点。另生成 3 组大样例，最大一组同样取满 $n=m=500000$。

构造覆盖 $n=1$、长度 2/3、$k=r-l$ 恒等、只改一项的 $k=r-l-1$、$r=n$ 与 $r=n-1$、单点仿射、零/单位/取负仿射、标记复合、素数及 2 幂附近长度、小周期、大周期、非整周期尾部、不同偏移、嵌套共享与回收。`preserved_seed` 长期保留前半区的大随机种子，专门防止小周期覆盖迅速抹平输入结构。

4 秒与 512 MiB 下，直接模拟实测 **40 分**，专用部分分实测 **70 分**，详见 `review/score_brute.json`、`review/score_partial.json`。评分遇到一组首个失败点后跳过同组剩余点，因此记录中 `skipped_after_group_failure` 不表示运行该点失败。满分 WBLT 与 AVL 都在全部 40 个正式点及 3 个大样例上通过。

## 一键生成

从任意工作目录均可运行；省略参数时输出至联考目录的 `data/calibration` 与 `down/calibration`：

```bash
bash /absolute/path/dev/calibration/gen/genall.sh
bash /absolute/path/dev/calibration/gen/genall.sh '/custom/data dir' '/custom/large sample dir'
```

相对自定义路径按调用时的当前目录解释。脚本自行定位 Git 根目录、编译构造器、testlib 校验器及三个核对程序；需要 `bash`、`g++`、`python3` 和 Git。每点先校验输入分组，再在各自 4 秒限制内运行 WBLT 与独立 AVL，检查答案数量、规范模数剩余类及逐项一致；第 1 组另与暴力程序比较。

全部检查成功后才复制声明的 43 对文件：正式点为 `01_01.in/ans` 至 `05_11.in/ans`，大样例为 `calibration1.in/ans` 至 `calibration3.in/ans`。只覆盖这批明确命名的文件，不删除目标目录中的其他文件；某点检查失败时原发布数据保持原状。临时构建目录由脚本创建并在退出时自行清理。

本次已从 `/tmp` 执行默认路径全量生成；又以含空格的两个自定义目录重跑全量生成，86 个文件 SHA-256 与默认结果一致，预置的 4 个无关文件均保留。见 `review/generation.json`、`review/reproducibility.json`。

## 正确性与工具验收

Python oracle 直接从题目窗口和递推期望答案；WBLT 开启 `AUDIT`，独立 AVL 开启 `AVL_AUDIT`，与 C++ 暴力、专用部分分四路核对。全部 **1210 例、239,264 次操作、181,275 个答案**一致，包含 1200 个随机实例和 10 个固定边界实例。另完成 31,302 次校准公式穷举、41,823 次仿射公式穷举、5000 次随机公式检查。

共享结构通过 100,000 次随机操作压力；穷举至 14 个逻辑叶子的 11,110 种平衡形状，覆盖 156,955 次分裂重拼、29,983 次拼接、15,900 次共享重复。所有结构审计、引用计数、平衡和矩断言通过。

testlib 正反例与生成策略共有 84 项，其中包含 59 种生成参数组合，错误分组、超界 $n,m$、非法窗口、缺少查询等全部按预期拒绝；输出数量/模数范围检查与提交输出 token checker 各有 8 组正反例。每个正式输入已使用 `--group` 校验；分组采用 `validator.group()` 读取 testlib 原生选项。专用部分分另有 600 个随机实例和 3 个公开样例验收记录。AddressSanitizer、UndefinedBehaviorSanitizer 与 WBLT 结构审计还通过 210 例、41,074 次操作、31,215 个答案，未报告内存或未定义行为错误，见 `review/sanitizers.json`。原始记录见 `review/oracle_validation.json`、`structural_tests.json`、`exhaustive_tests.json`、`tool_tests.json`、`partial_fuzz.json`。

## 性能与限制选择

测试环境：WSL2 Linux x86-64，g++ (Ubuntu 10.5.0-1ubuntu1~20.04.1+esm1) 10.5.0，`-std=c++17 -O2 -DNDEBUG`。机器信息和逐项汇总见 `review/validation_summary.json`。时长为含输入输出的单进程墙钟时间；额外 7 类策略均使用 $n=m=500000$，分别编译两份满分解运行并比较输出。

| 范围 | WBLT 最慢 | 独立 AVL 最慢 |
| --- | ---: | ---: |
| 40 个正式点 | 1.783 秒 | 1.996 秒 |
| 7 类最大规模策略 | 2.83 秒 | 2.92 秒 |
| 策略测试峰值 RSS | 32.14 MiB | 35.13 MiB |

因此选择 4 秒满足“时限小于 5 秒”，并保留大规模对一般模拟和不合理展开的区分。目标 OJ 的 CPU 与编译器可能不同，上线时应以这两份程序及保留种子策略复核机器性能；不把 WSL 的耗时当作其他机器的保证。可运行 `bash review/benchmark_scale.sh` 复测。
