# 《存储空间》出题报告

比赛文件名：`storage`。建议时限 10 秒，空间限制 256 MiB，C++17。

本目录整理自 `flexible_storage_v2`。题意、十档分值、完整算法与参考程序保持原设计；生成器和验证器改为 C++ testlib，统一使用 Git 根目录的 `testlib.h`。本版使用 testlib 随机序列重新生成全部数据，因此固定参数可复现本版输入，但输入哈希不与原 Python 生成器版本相同。

## 文件与使用

- [statement.md](statement.md)：正式题面与题面样例。
- [sol/solution.md](sol/solution.md)：完整证明、实现说明及部分分算法。
- [sol/std_hld.cpp](sol/std_hld.cpp)：重链矩阵、两类计数最小值与四种乘积，时间 $O(n+q\log^2 n)$，空间 $O(n)$。
- `sol/partial_recompute.cpp`：每次全树重算；`partial_height.cpp`：只重算受影响祖先；`partial_short.cpp`：长度不超过 2 的独立组件算法。
- `gen/{gen.cpp,val.cpp,genall.sh}`：testlib 生成器、严格验证器、全量生成脚本。
- `review/`：独立枚举暴力、扫描线快照解、复现脚本、参数、核验报告及历史报告。
- 正式数据位于本题目录的 `../../data/storage/`，格式为 `01_01.in/ans` 至 `10_09.in/ans`；下发大样例位于 `../../down/storage/`，名称为 `storage1.in/ans`、`storage2.in/ans`。

在本题目录运行：

```bash
bash gen/genall.sh
python3 review/verify.py --data
python3 review/test_genall.py
python3 review/benchmark.py
python3 review/mutation_test.py
```

`genall.sh` 可从任意当前目录调用，默认路径始终相对本题目录定位。可传入正式数据目录和大样例目录；显式相对路径相对调用时的当前目录解释：

```bash
bash /absolute/path/dev/storage/gen/genall.sh /path/to/data /path/to/samples
```

脚本先编译生成器、验证器和标准解，对每个输入执行所属分组验证，再生成并检查答案。只替换本题计划中的输入、答案文件，不清理目标目录，不删除无关文件；暂存文件建在目标目录内，支持将结果输出到 Windows 挂载盘。生成参数见 [review/cases.json](review/cases.json)，文件哈希与生成耗时见 [review/generated_manifest.json](review/generated_manifest.json)。验证命令需要 `g++`、Python 3.8+；性能复测还需要 `/usr/bin/time`。

单独编译与运行：

```bash
g++ -std=c++17 -O2 -I "$(git rev-parse --show-toplevel)" gen/gen.cpp -o /tmp/storage_gen
g++ -std=c++17 -O2 -I "$(git rev-parse --show-toplevel)" gen/val.cpp -o /tmp/storage_val
/tmp/storage_gen 100000 100000 zigzag_comb adversarial large 2026103101 random > /tmp/storage.in
/tmp/storage_val --group 10 < /tmp/storage.in
```

生成器依次接受 `n q shape mode weights seed numbering`。形态有精确中点、随机、左右梳状、交替梳状、右重、深度不超过 30 七类；操作有随机、混合、仅节点区间、只加、整段与单点循环、共享引用循环、针对性构造、短区间八类；费用有单位、随机、大费用、父子平局四类；节点编号支持随机、逆序、先序。构造与输出全部迭代执行，允许线性树深。

验证器通过 `--group 1` 至 `--group 10` 选择额外约束，默认第 10 档；严格检查行格式、全树连通、节点入度、孩子合法、叶子数、根编号、正费用、所有区间与增删合法性，并拒绝额外输入。

## 命题思路与正确性要点

本题面向已掌握树链剖分和动态树 DP 的选手。任意树形下，一个区间的规范分解可以有线性多个块，因此不能直接枚举这些块；独立正费用使“购买当前节点”与“购买子孙节点”的取舍及平局计数成为必要部分。

树区间具有层级性。一个目标区间可唯一分解成完整包含于它的极大节点区间；可行存储方案恰好需要完整覆盖所有仍被引用的规范块。必须记录引用次数，删除一个原区间不能清掉其他原区间共享的需求。

令 $X_v$ 表示满足严格子孙需求的最优状态，$B_v$ 表示同时覆盖整个节点区间的最优状态，$A_v$ 根据自身需求是否非零选择 $X_v$ 或 $B_v$。状态为“最小费用、方案数”，独立合并时费用相加、方案数相乘；同费用分支的方案数相加。内部节点满足

$$
X_v=A_l\otimes A_r,\qquad
B_v=(w_v\otimes X_v)\oplus(B_l\otimes B_r).
$$

两个 $B_v$ 分支分别选择和不选择 $v$，集合互不相交，故没有重计数。正费用保证仅满足严格子孙需求时不需要额外选择 $v$。叶子处 $X=(0,1)$、$B=(w,1)$，由树高归纳得到静态 DP 正确性。

取两个端点的最高包含祖先，再沿两条边界路径修改适当方向的兄弟需求，可将线性多个块压缩成 $O(\log n)$ 段重链区间。将轻孩子的需求次数存于父节点位置，并保存两类方向各自的计数最小值及四种假想矩阵乘积，使整段加减只需改变最小值和懒标记。同一次操作的链头变化按深度统一传播，得到 $O(\log^2 n)$ 单次复杂度。完整边界证明与矩阵公式见题解。

## 子任务与数据设计

采用独立子任务计分，每档全部测试点通过才得分，共 100 分。

| 档 | 分值 | 点数 | 额外限制及目的 |
| --- | ---: | ---: | --- |
| 1 | 5 | 4 | $n\le7,q\le100$，枚举存储节点集合 |
| 2 | 10 | 4 | $nq\le2\times10^6$，全树重算 |
| 3 | 10 | 4 | 精确中点分割，限制树高 |
| 4 | 5 | 4 | 叶子深度不超过 30，根深度为 0 |
| 5 | 10 | 4 | 每个操作恰为某个节点区间，普通动态树 DP |
| 6 | 10 | 4 | 每个内部节点左孩子为叶子，单方向长链 |
| 7 | 10 | 4 | 右子树叶子数不少于左子树，保留多链传播 |
| 8 | 10 | 4 | 只有加入，可摊还处理首次激活 |
| 9 | 5 | 4 | 操作长度不超过 2，独立线性组件算法 |
| 10 | 25 | 9 | 无额外约束 |

正式数据共 45 点，另有两个 $n=q=100000$ 的下发大样例。合计 3,923,500 次操作。主要覆盖随机非拓扑编号、最大深度左右梳状树、重边方向交替、多层轻链传播、多个原区间共享规范块及不同删除顺序、整段与端点单点相互影响、反复清空、父子正费用平局、方案数取模、大费用 64 位累加、接近区间全集的只加操作。

第 9 档采用短区间条件，没有使用早期的 `q<=2000` 方案。第 3 档要求精确中点，不能只保证大致平衡；第 6、7 档都以实际树结构检查，不能依赖节点编号。

## 本次整理的实测核验

本次已实际编译全部 8 份 C++ 工具及参考程序，并执行：

- 899 组生成器/验证器检查，覆盖全部小规模形态、操作、费用组合和三类最大深度树；固定种子复现通过。
- 6 种全局非法输入、9 档限制的逐档违约输入，以及非法分组编号全部被拒绝。验证器显式调用 `prepareOpts`，确保 Git 根 testlib 的 `--group` 真正生效。
- 262 组整程序对拍，共 638,815 次操作；其中 520,515 次操作还与直接枚举全部存储节点集合的暴力核对。包括小规模活动区间全集的 Gray 遍历、180 组随机测试、7 组中型测试。
- 45 组正式点与 2 组大样例全部重新生成，全部通过所属分组检查、答案格式检查和 SHA-256 复核。
- 全部 47 个文件经独立扫描线/Fenwick 快照解核对。该解从当前活动原区间集合重建规范需求，不复用标准解的动态 HLD 矩阵；检查前 8 次、均匀 20 个时点与末次，去重后通常 28 次。适用分组还逐行与枚举、全树重算、受影响祖先或短区间程序比较。
- `genall.sh` 已从 `/tmp` 调用，测试默认目录、含空格的自定义目录、WSL 到 Windows 挂载盘的跨文件系统输出及无关哨兵文件保留。
- 12 类故意错误的标准解变体在 37 组针对性测试中全部出现实际错误输出，也全部被当前正式数据抓住；题面样例逐字比较通过。复现命令为 `python3 review/mutation_test.py`，结果见 [review/mutations.json](review/mutations.json)。

当前结果见 [review/verification.json](review/verification.json)、[review/data_verification.json](review/data_verification.json)、[review/genall_paths.json](review/genall_paths.json)。五组代表性数据各重复三次，在 WSL2、g++ 10.5.0 下最慢中位数为 2.80 秒，峰值 RSS 为 104,380 KiB；详见 [review/benchmark.json](review/benchmark.json)。实际评测平台应按其处理器与编译器复测 10 秒、256 MiB 限制。

`review/previous_reports/` 保存原包的历史验收、错误实现测试、分数及 Windows 性能记录；其中路径、输入哈希和运行环境属于原 Python 生成器版本。本次新数据的核验以 `review/` 顶层报告为准。更详细的构造动机与实现不变量保存在 [review/author_notes.md](review/author_notes.md)。
