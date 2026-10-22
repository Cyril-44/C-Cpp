# 核验与复现

从任意目录运行 `python3 /absolute/path/dev/storage/review/verify.py --data`，编译全部参考程序，检查生成器、严格分组验证器、262 组整程序对拍，并核验默认生成的全部正式数据和大样例。

- `brute.cpp` 独立枚举存储节点集合，仅适用于小规模。
- `snapshot_oracle.cpp` 从活动原区间集合通过扫描线/Fenwick 重建规范计数，再做全树静态 DP；不依赖标准解的动态矩阵。
- `verify.py --data-only` 只编译并检查已经生成的最终数据。
- `test_genall.py` 用小型独立夹具测试任意 cwd、默认路径、自定义空格路径和无关文件保留，不修改真实数据及其清单。
- `mutation_test.py` 重建原 12 类错误实现，使用当前 testlib 生成的小测试及当前 45 个正式输入复测，报告为 `mutations.json`。
- `benchmark.py` 实际运行五组代表性数据，每组三次，比较答案并记录时间、峰值 RSS。
- `check_output.py` 检查答案行数、整数格式及值域。
- `cases.json` 固定生成参数；`generated_manifest.json` 记录本次输入、答案哈希及生成时间。
- `samples/` 保存题面样例供人工和程序复核。
- 顶层 `verification.json`、`data_verification.json`、`genall_paths.json`、`benchmark.json` 为本次整理的实测报告。

`previous_reports/` 为原 v2 包的历史报告，保留原始内容用于追溯，包括原 Windows 测量、部分分实测和变异测试。报告里的 `tools/`、`solutions/`、`reports/` 路径指原包布局；对应旧 Python 随机序列与旧文件哈希，不能作为本版新数据的验证结果。原无用打包脚本、缓存与旧数据副本未导入。更详细的命题设计见 `author_notes.md`。
