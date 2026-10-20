# 《自由分割，按价存储》题目包

版本：v2 初稿，2026-10-21。输入树允许任意深度，存储节点具有独立正整数费用。

建议先阅读 [验收报告](reports/acceptance.md)，其中汇总了数据数量、实测分数、正确性核验和本机性能。

## 内容

- `statement.md`：正式题面、样例及十档子任务。
- `editorial.md`：完整算法、正确性说明、复杂度与各档部分分算法。
- `config/subtasks.json`：全局限制、分值和子任务条件。
- `solutions/std.cpp`：完整标准解，复杂度为 $O(n+q\log^2 n)$ 时间、$O(n)$ 空间。
- `solutions/recompute.cpp`：每次全树重算的参考程序。
- `solutions/brute.cpp`：小规模枚举存储集合的独立暴力程序。
- `solutions/bounded_height.cpp`：只重算受影响节点的 $O(n+qh)$ 解法。
- `solutions/short.cpp`：长度不超过 2 时的独立 $O(n+q)$ 组件解法。
- `tools/val.cpp`：输入及子任务验证器。
- `tools/checker.cpp`：输出检查器。
- `tools/gen.py`、`tools/case_plan.json`：固定种子生成器和完整用例参数。
- `data/private/`：45 组正式输入与标准答案。
- `data/public/`：2 组 $n=q=100000$ 的下发大样例及答案。
- `samples/`：题面样例。
- `tools/snapshot_oracle.cpp`：使用扫描线、树状数组重建状态的独立大数据检查器。
- `tools/verify.py`、`tools/verify_tools.py`、`tools/verify_data.py`：整程序对拍、工具测试、正式数据核验。
- `tools/mutation_test.py`：检查常见错误实现能否被数据发现。
- `tools/benchmark.py`、`tools/judge.py`：性能测试与本地分组计分。
- `tools/package.py`：生成出题人包、选手包及 SHA-256 校验文件。
- `docs/author_notes.md`、`reports/data_design.md`：出题说明与数据覆盖设计。

## 评测约定

- 单个输入文件包含一组测试。
- $1\le n,q\le10^5$，节点数为 $2n-1$，费用为 $[1,10^9]$ 内的整数。
- 根节点编号为 $1$，其他编号任意；叶子的区间序号由先左后右的遍历顺序生成。
- 全部增删操作保证合法；删除直接给出区间，不使用历史操作编号。
- 每次输出最小费用与最优节点集合数。最小费用使用 64 位整数，方案数对 $998244353$ 取模。
- 十个子任务独立计分，组内全部通过方可得分，总分 $100$。
- C++ 参考程序采用 C++17。

建议时间限制 **10 秒**，内存限制 **256 MiB**。本机性能记录见 `reports/benchmark.json`；迁移到实际评测机器后仍需复测。相关配置保存在 `config/subtasks.json`。

## 子任务约定

第 9 档采用“每个操作区间长度不超过 2”，分值为 5 分，不采用早期讨论中的 `q <= 2000` 条件。它有独立的加权双叶组件算法，时间为 $O(n+q)$。

第 3 档必须是题面定义的精确中点分割；第 4 档的深度从根的 0 开始计算；第 6 档只要求每个内部节点的左孩子为叶子，不要求节点编号顺序；第 7 档比较左右子树的叶子数。

## 核验重点

小规模正确性应由标准解、全树重算和枚举存储集合三者交叉验证。重点覆盖非拓扑顺序编号、线性深度树、正费用平局、规范分解节点被多个原区间共同引用，以及删除后仍有引用的情况。

正式性能测试应包含左右梳状树、重孩子方向交替变化的树、最大规模边界路径更新和反复清空需求的操作。数学推导或局部公式核验不能替代整程序压测；实际通过情况以测试记录为准。

## 使用方法

需要 C++17 编译器及 Python 3.10 或更高版本。Python 工具只使用标准库。以下命令在题目包根目录执行，默认将可执行文件写到 `build/`：

```text
python tools/build_data.py
python tools/verify.py
python tools/verify_tools.py --build-dir build/tool_tests
python tools/verify_data.py
python tools/mutation_test.py --build-dir build/mutations
python tools/benchmark.py
python tools/judge.py --name std
python tools/audit.py
python tools/package.py
```

`build_data.py` 会重新生成输入与答案；已下发的数据可直接使用，不必先重新生成。它会调用验证器检查每个分组的限制。`--resume` 只有在生成参数、源码哈希和现有数据哈希均吻合时才复用文件。

`judge.py` 在某个子任务首次失败后停止该组，其他组继续运行；它提供本地评分，不是隔离执行环境，也不强制限制进程内存。实际比赛请使用评测系统执行时限和内存限制。

单独使用验证器和检查器：

```text
build/val --subtask 7 --input data/private/s07_01.in
build/checker input.in contestant.out standard.ans
build/snapshot_oracle input.in standard.ans 20
```

Windows 下可执行文件带 `.exe` 后缀。检查器返回 0 表示通过，1 表示选手输出错误，3 表示参数或答案文件等评测配置错误。

## 发布与文件校验

`FILE_SHA256SUMS.txt` 记录出题人包内文件校验值；`manifest.json` 记录全部正式数据和大样例的参数、哈希与生成时运行时间。

出题人压缩包包含题解、程序及全部私有数据。选手压缩包只包含题面、题面样例和两个大样例；其中大样例映射到 `samples/large/`，不包含私有评测数据或出题源码。
