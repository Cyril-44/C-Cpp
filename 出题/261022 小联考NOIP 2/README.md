# 261022 小联考 NOIP 2：题包整理报告

工作目录：`/home/cyril/code/出题/261022 小联考NOIP 2`（WSL）。

已将指定来源的 LRUD、storage、calibration 整理进 `dev/`，并统一现有 ring。四题均包含 `gen/{gen.cpp,val.cpp,genall.sh}`、`review/`、`sol/`、`README.md` 与 `statement.md`。生成器及校验器使用 Git 根目录 `/home/cyril/code/testlib.h`，没有复制另一版本的头文件。旧 ring 重复源码、原数据目录及冗余脚本已迁移或删除。未创建 Git 提交。

## 数据与时限

| 题目 | 正式点 | 大样例 | 时限 | 空间 |
| --- | ---: | ---: | ---: | ---: |
| ring | 30 | 5 | 5 秒 | 128 MiB |
| calibration | 40 | 3 | 4 秒 | 512 MiB |
| lrud | 37 | 3 | 1 秒 | 512 MiB |
| storage | 45 | 2 | 10 秒 | 256 MiB |
| 合计 | 152 | 13 | | |

正式数据已经生成到比赛根目录 `data/题目名/`，命名为 `01_01.in/ans` 等；大样例已经生成到 `down/题目名/`，命名为 `ring1.in/ans`、`lrud1.in/ans` 等。共 330 个输入、答案文件，均检查了配对、命名、LF 换行及非空；各题的清单、随机种子和复核报告位于其 `gen/` 或 `review/` 中。`package-audit.json` 保存分组点数、大小与汇总哈希。

## 一键重生成

```bash
cd '/home/cyril/code/出题/261022 小联考NOIP 2'
for p in ring calibration lrud storage; do
  bash "dev/$p/gen/genall.sh"
done

# 单题可指定正式数据和大样例目录，也可从任意 cwd 用绝对脚本路径调用
bash dev/lrud/gen/genall.sh '/custom/final data' '/custom/large samples'
```

默认 `../../data/题目名`、`../../down/题目名` 均相对题目目录 `dev/题目名` 解析；显式传入的相对路径相对调用时的 cwd。四题均实测从 `/tmp` 运行、包含空格的自定义目录、保留无关文件。脚本会校验子任务限制，并生成对应答案。

**LRUD 必须配置特殊评测**：使用 `dev/lrud/review/checker.cpp`，以 C++17 `-O2 -I /home/cyril/code` 编译；不能对构造串做全文比较。checker 验证最优值、路径长度、八邻接、每格恰好访问一次、起终点、正右步计数及额外输出。

## LRUD 的五档

独立计分组为 10、15、20、20、35 分，分别限制 $N\le3$、$N\le4$、小规模或奇数、小规模或 $N\not\equiv0\pmod4$、无限制。提供 DFS、子集 DP、奇数蛇形、两列条带及通用四竖步构造；实测累计得分恰为 **10、25、45、65、100 分**。

采用所给 `maximum_path.cpp` 的“求最大值并输出路径”版本；原 `solution.cpp` 的给定 $N,K$ 版本不混用。全 $N=2..1000$ 验证了 333,833,499 个格子；独立小规模最优值 DP、SPJ 正反例和部分分逐点矩阵均通过。完整最优值证明附于 LRUD README，并经独立审阅。

## calibration 采用指定的 Codex linear 版本

题目定义为**窗口和构成等差数列，保留前 $k+1$ 项**，合法 $1\le k\le r-l$；与原合订题面中二阶差分恒定、保留 $k+2$ 项的旧版本不同。已将单题题面、合订 Markdown、TeX、PDF 统一为指定版本，包括全部样例。

范围从 $n,m\le3\times10^6$ 缩至 **$n,m\le5\times10^5$**，时限定为 **4 秒**。全部 40 个正式点及 3 个大样例经过 WBLT、独立 AVL 交叉核对；小组另与暴力比较。WSL 实测正式数据最慢为 1.783 / 1.996 秒，额外七类满规模策略最慢为 2.83 / 2.92 秒，内存约 32 / 35 MiB。直接模拟与专用部分分实测为 40 / 70 分。另通过 1210 例四路审计、公式和共享结构检查。

上述耗时来自当前 WSL2、i7-14650HX、g++ 10.5.0、C++17 `-O2`；目标评测机应依据其 CPU 与编译器复核 4 秒时限。

## 其他验收

- ring：5910 棵小树穷举、400 随机、40 个生成模式用例，以及全部正式点和大样例的独立矩阵解交叉与固定种子复现通过；峰值内存约 46 MiB。
- storage：899 组生成器/校验器测试，262 组程序对拍、638,815 次操作；全部 47 份正式点/大样例经独立扫描线快照解抽时点复核，适用子任务追加逐行独立解比较。代表性数据三次计时的最慢中位数 2.80 秒，峰值内存约 102 MiB。
- 修复 Git 根 testlib 中 `registerValidation` 不自动调用 `prepareOpts` 导致自定义分组参数失效的问题；所有题目均已验证违规分组被拒绝。
- 合订题面 `dev/statement/{A,B,C,D}.md` 与各题 `statement.md` 逐字节一致。`main.tex` 已编译成功，PDF 逐页渲染检查，无 overfull、underfull 或 missing-character 日志。

各题详细出题报告、证明、生成方式和复现命令以 `dev/题目名/README.md` 为入口；本报告只汇总已完成的整理与验收。
