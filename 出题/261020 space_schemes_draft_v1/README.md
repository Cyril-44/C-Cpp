# 《空间相同，方案不同》完整题目包 · 初稿 v1

版本日期：2026-10-20。题目类型：传统输入输出、独立子任务计分。

这是一份供出题人验收的通用题包。源代码使用 C++17，生成、验证和打包脚本使用 Python 3.10 或更高版本的标准库，不依赖 testlib 或第三方 Python 包。

出题动机来自讨论中的 P17564「FAOI-R15」请注意你的空间常数。本题保留层级区间的共享存储模型，将“所有需求子集的空间之和”改为“在线增删需求后的最小空间及最优存储集合计数”。发布署名和比赛归属由出题人填写。

## 建议先看

1. [验收摘要](reports/acceptance.md)：当前包的文件、验证结果、实测性能及需要最终确定的发布参数。
2. [正式题面](statement.md)：含 10 档部分分，总计 100 分。
3. [完整题解](editorial.md)：正确性、计数无重证明、复杂度、各档部分分做法。
4. [数据设计](reports/data_design.md)：各组测试点、生成模式和针对的错误。
5. [标准程序](solutions/std.cpp)：`O(n + q log n)` 时间，`O(n)` 空间。

## 初稿评测设置

- `n <= 100000`，`q <= 300000`。
- 方案数模数：`998244353`。
- 建议时间限制：**3 秒**；建议内存限制：**256 MiB**。
- 10 个子任务独立计分，组内全部测试通过才获得该组分数，不设跨组依赖。
- 分值依次为 **5、7、10、5、8、10、10、10、10、25**。
- 测试点组别与种子见 [config/cases.json](config/cases.json)，评分权威配置为 [config/subtasks.json](config/subtasks.json)。

时限是初稿建议，实测机器信息见 [reports/benchmark.json](reports/benchmark.json)。上线前在目标评测机上复测；语言倍率由比赛平台配置。本地评测脚本实施时间限制，不模拟操作系统级内存限制。

## 文件结构

```text
statement.md                 正式题面，含全部部分分约束
editorial.md                 完整题解及部分分算法
solutions/
  std.cpp                    完整标准程序
  brute.cpp                  独立枚举存储集合的暴力，n <= 8
  recompute.cpp              每次全树重算的正确慢程序
  disjoint.cpp               活动需求两两不交的部分分程序
  whole_singleton.cpp        整段加单点的独立公式程序
  short.cpp                  长度 <= 2 的独立组件程序
tools/
  gen.py                     固定种子的合法输入生成器
  val.cpp                    输入、操作合法性和子任务性质校验器
  checker.cpp                严格整数 token 答案检查器
  snapshot_oracle.cpp        在大数据中分散检查点全树重算
  build_data.py              生成全部数据、运行标程产生答案
  verify.py                  暴力对拍、全量/快照参考解核对、评测工具自测
  mutation_test.py           常见错误程序的数据检出测试
  benchmark.py               性能与内存基准测试
  judge.py                   按独立子任务进行本地评分
  make_reports.py            生成验收摘要和数据设计索引
  package.py                 生成主办方包、选手下发包及 SHA-256
  tests/                     评测工具正反测试
config/                      子任务、分值、数据生成参数
data/                        43 个正式测试点的 .in/.ans
samples/
  sample1.in/.ans             题面样例
  large/large1.in/.ans        n=100000, q=300000，下发大样例一
  large/large2.in/.ans        n=99999, q=200000，下发大样例二
reports/                     验证、性能、错误程序检出与评分报告
manifest.json                测试点清单、分组、文件大小和 SHA-256
contestant_README.md          选手下发包说明
```

## 从源码复现

以下命令在本题包根目录运行。Windows 可用 `python`，其他系统也可用 `python3`。编译器默认从环境变量 `CXX` 读取，否则使用 `g++`。

```sh
python tools/build_data.py
python tools/verify.py
python tools/mutation_test.py
python tools/benchmark.py
python tools/make_reports.py
python tools/package.py
```

`build_data.py` 会覆盖包内生成的输入、答案和清单，但不修改题面、题解与源代码。可以用 `--build-dir PATH` 指定编译产物目录，用 `--cxx g++` 指定编译器。其余验证工具也支持 `--build-dir`；运行时应使用同一目录。

单独生成一个输入：

```sh
python tools/gen.py --n 100000 --q 300000 --seed 1001 --mode random --output example.in
```

单独校验第 7 个子任务的数据：

```sh
build/val --subtask 7 --input example.in
```

Windows 的程序名为 `build/val.exe`。校验器也支持从标准输入读取，以及 `--max-n`、`--max-q`、`--max-product`、`--exact-n`、`--property`。重复的限制和子任务限制按交集检查，不能用宽松参数绕过子任务限制。

本地评分：

```sh
python tools/judge.py build/std --report reports/std_score.json
```

Windows 使用 `build/std.exe`。选手程序必须通过标准输入输出交互。每个测试点启动独立进程。

## Checker 接口

```text
checker input_file contestant_output answer_file
```

输入顺序与传统 checker 一致。返回码：`0` 为 AC，`1` 为 WA，`3` 为评测文件或调用错误。检查器接受任意合法空白，拒绝额外或缺失的 token、非整数、越界值和错误答案；最小空间和方案数分别核对。它不实现评分，分值由子任务分组决定。

本题不是需要比较构造方案的特殊评测题，也可在比赛平台上直接使用等价的整数 token 比较器。

## 发布与导入

`package.py` 在指定的输出目录生成：

- `space_schemes_organizer_draft_v1.zip`：完整主办方题包。
- `space_schemes_contestant_draft_v1.zip`：仅含题面、选手说明、题面样例和两组大样例及答案。
- `space_schemes_archives_sha256.txt`：两个压缩包的 SHA-256。

给选手下发第二个压缩包。它按白名单打包，不包含标程、题解、生成器或正式评测数据。

题包采用通用目录与 JSON 分组配置，没有假定某一比赛平台的导入格式。导入时应保持 10 个独立子任务的全有或全无评分语义，以及各测试点的所属组别。
