# 叶环独立集

一份可直接生成、校验并评测的 OI 题包。

## 目录

- `statement.md`：中文题面和样例。
- `solution/`：标准程序、题解以及各档参考程序。
- `validator.cpp`、`gen.cpp`：testlib 输入校验器和数据生成器。
- `make_data.sh`：编译工具并生成分档评测数据和公开样例。
- `judge.sh`：运行选手程序并用 `diff` 比对所有输入的答案。
- `data/subtask*`：生成后的官方数据，每个 `.in` 都有同名 `.ans`。
- `samples/`：题面样例及几组较大的公开测试。

## 生成数据

需要 `g++`、Bash、`awk`、`diff`、GNU coreutils（提供 `timeout`）和 `~/C-Cpp/testlib.h`：

```bash
bash make_data.sh
```

可通过 `TESTLIB_INCLUDE_DIR` 指定 testlib.h 所在目录。数据生成器的随机树使用显式固定种子，重新运行会得到相同数据。

## 评测程序

```bash
bash judge.sh ./solution/std data
```

脚本递归遍历指定目录中的 `.in`，优先匹配同名 `.ans`，没有时匹配同名 `.out`。每个测试默认限时 5 秒，可用 `TIME_LIMIT=秒数` 调整。输出比较会忽略每行末尾的空白字符，并统一文末换行；其他差异由 `diff -u` 报告。程序非零退出、超时或答案不匹配时脚本返回非零状态。

## 编译参考程序

```bash
g++ -std=c++17 -O2 -pipe solution/std.cpp -o solution/std
g++ -std=c++17 -O2 -pipe solution/brute.cpp -o solution/brute
g++ -std=c++17 -O2 -pipe solution/few_leaves.cpp -o solution/few_leaves
g++ -std=c++17 -O2 -pipe solution/star.cpp -o solution/star
```
