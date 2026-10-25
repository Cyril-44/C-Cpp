# 261022 小联考 NOIP 2：题包整理报告

工作目录：`/home/cyril/code/出题/261022 小联考NOIP 2`（WSL）。

已将指定来源的 LRUD、storage、calibration 整理进 `dev/`，并统一现有 ring。四题均包含 `gen/`、`review/`、`sol/`、`README.md` 与 `statement.md`。生成器与校验器为 `gen/gen.cpp`、`gen/val.cpp`；ring、LRUD 使用纯 Bash 的 `gen/genall.sh`，calibration、storage 直接使用 `gen/genall.py`。数据生成代码和参数清单集中在 `gen/`，`review/` 保留核心验题源码、脚本与独立样例，出题报告和验题结论集中在各题 `README.md`。生成器及校验器使用 Git 根目录 `/home/cyril/code/testlib.h`，没有复制另一版本的头文件。旧 ring 重复源码、原数据目录及冗余脚本已迁移或删除。未创建 Git 提交。

## 数据与时限

| 题目 | 正式点 | 下发样例 | 时限 | 空间 |
| --- | ---: | ---: | ---: | ---: |
| ring | 30 | 6（1 题面 + 5 大样例） | 1 秒 | 128 MiB |
| calibration | 40 | 6（3 题面 + 3 大样例） | 1 秒 | 512 MiB |
| lrud | 37 | 1（题面样例） | 1 秒 | 512 MiB |
| storage | 45 | 3（1 题面 + 2 大样例） | 10 秒 | 256 MiB |
| 合计 | 152 | 16 | | |

正式数据位于比赛根目录 `data/题目名/`，命名为 `01_01.in/ans` 等；下发样例位于 `down/题目名/`。各题题面样例按顺序放在下发目录的编号最前面，剩余大样例顺延。LRUD 下发当前题面的 `lrud1.in/ans` 和选手使用的简易 checker，保留当前不额外生成大样例的安排。当前数据包共 336 个输入、答案文件；生成参数清单和随机种子位于各题 `gen/`，独立暴力、参考实现及验题入口位于 `review/`。

## 一键重生成

```bash
./dev/build.sh

# 单题可指定正式数据和下发目录
bash dev/lrud/gen/genall.sh '/custom/final data' '/custom/samples'
python3 dev/calibration/gen/genall.py '/custom/final data' '/custom/samples'
python3 dev/storage/gen/genall.py '/custom/final data' '/custom/samples'
```

总构建脚本会从 Markdown 更新并编译题面，根据各题入口直接调用 `genall.py` 或纯 Bash `genall.sh`，生成四题数据，并复制 PDF 与 LRUD 混淆 checker.c 到 `down/`。ring 的正式目录与下发目录仅生成 `.in/.ans`，不再生成 manifest。各题生成脚本默认路径相对题目目录解析；显式传入的相对路径相对调用时的 cwd。脚本会校验子任务限制并生成答案。

**LRUD 必须配置特殊评测**：评测机使用 `dev/lrud/checker/checker.cpp`，这是依赖 testlib 的 checker；不能对构造串做全文比较。它验证最优值、路径长度、八邻接、每格恰好访问一次、起终点、正右步计数及额外输出。选手仅下发已有混淆产物 `down/lrud/checker.c`，使用 `gcc checker.c -lm -ldl -pthread -o checker` 编译、`checker input.txt output.txt` 运行；生成脚本会清理旧版明文 `down/lrud/checker.cpp`。明文 C++ 与 JavaScript checker 及混淆构建文件均不下发。

## LRUD 的五档

独立计分组为 10、15、20、20、35 分，分别限制 $N\le3$、$N\le4$、小规模或奇数、小规模或 $N\not\equiv0\pmod4$、无限制。提供 DFS、子集 DP、奇数蛇形、两列条带及通用四竖步构造；实测累计得分恰为 **10、25、45、65、100 分**。

采用所给 `maximum_path.cpp` 的“求最大值并输出路径”版本；原 `solution.cpp` 的给定 $N,K$ 版本不混用。全 $N=2..1000$ 验证了 333,833,499 个格子；独立小规模最优值 DP、SPJ 正反例和部分分逐点矩阵均通过。完整最优值证明附于 LRUD README，并经独立审阅。

## calibration 采用指定的 Codex linear 版本

题目定义为**窗口和构成等差数列，保留前 $k+1$ 项**，合法 $1\le k\le r-l$；与原合订题面中二阶差分恒定、保留 $k+2$ 项的旧版本不同。已将单题题面与合订题面 统一为指定版本，包括全部样例。

calibration 的数据上限现为 **$n,m\le10^5$**，三个规模档为 1000、10000、100000，题面时限 **1 秒**。40 个正式点及 3 个大样例按新范围重新生成，另附 3 个原始题面样例；磁盘中原先残留的 500000 规模数据已替换为当前参数对应数据；生成脚本会用 WBLT 与独立 AVL 逐点交叉核对，第 1 组另与暴力比较。此前的 500000 规模性能数据属于旧范围，不代表当前正式数据。

上述耗时来自当前 WSL2、i7-14650HX、g++ 10.5.0、C++17 `-O2`；目标评测机应依据其 CPU 与编译器复核题面时限。

## 其他验收

- ring：5910 棵小树穷举、400 随机、40 个生成模式用例，以及全部正式点和大样例的独立矩阵解交叉与固定种子复现通过；峰值内存约 46 MiB。
- storage：899 组生成器/校验器测试，262 组程序对拍、638,815 次操作；当前全部 48 份正式点/下发样例经独立扫描线快照解抽时点复核，适用子任务追加逐行独立解比较。代表性数据三次计时的最慢中位数 2.80 秒，峰值内存约 102 MiB。
- 修复 Git 根 testlib 中 `registerValidation` 不自动调用 `prepareOpts` 导致自定义分组参数失效的问题；所有题目均已验证违规分组被拒绝。
- 合订题面由 `dev/statement/build_statement.py` 直接读取各题 `statement.md` 生成；本次保留当前题面与构建方式，更新了 `dev/build.sh` 的数据生成入口分派。

`review/` 已清除历史报告、重复 JSON 配置、哈希清单与兼容包装；保留独立样例文件供验题直接读取。验题脚本默认把结果输出到终端，不在目录顶层积累运行报告。

各题详细出题报告、证明、生成方式和复现命令以 `dev/题目名/README.md` 为入口；本报告只汇总已完成的整理与验收。
