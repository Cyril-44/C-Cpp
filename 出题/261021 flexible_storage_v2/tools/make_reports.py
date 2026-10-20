#!/usr/bin/env python3
"""Render the acceptance summary from completed machine-readable reports."""
import json
from common import ROOT


def read(path):
    return json.loads((ROOT / path).read_text(encoding="utf-8"))


def main():
    manifest = read("manifest.json")
    differential = read("reports/differential.json")
    data = read("reports/data_verification.json")
    benchmark = read("reports/benchmark.json")
    mutations = read("reports/mutations.json")
    tools = read("reports/judge_tools.json")
    probe = read("reports/unbatched_performance.json")
    generator = read("reports/generator_tests.json")
    config = read("config/subtasks.json")
    total_bytes = sum(c["input_bytes"] + c["answer_bytes"] for c in manifest["cases"])
    max_median = max(c["median_seconds"] for c in benchmark["cases"])
    peak = max(c.get("peak_working_set_bytes") or 0 for c in benchmark["cases"])
    full_checked = sum(bool(c["full_output_references"]) for c in data["records"])
    score_rows = []
    for name, description in [
        ("std", "完整四种乘积与批量传播"),
        ("brute", "直接枚举存储集合，限制 n≤8"),
        ("recompute", "每次全树重算"),
        ("bounded_height", "仅重算受影响节点"),
        ("short", "长度≤2 的独立组件解法"),
    ]:
        report = read("reports/score_" + name + ".json")
        passed = "、".join(str(x["subtask"]) for x in report["subtasks"] if x["score"])
        score_rows.append(f"| `{name}.cpp` | {description} | {report['score']} | {passed} |")
    subtask_rows = []
    for s in config["subtasks"]:
        count = sum(c["subtask"] == s["id"] and not c["public"] for c in manifest["cases"])
        subtask_rows.append(f"| {s['id']} | {s['score']} | {count} | {s['description']} |")
    summary = f"""# 《自由分割，按价存储》v2 验收报告

日期：2026-10-21。完整题目包初稿已生成，供出题验收。

## 交付内容

- [完整题面](../statement.md)、[完整题解](../editorial.md)、[出题说明](../docs/author_notes.md)。
- [标准程序](../solutions/std.cpp)及 4 份不同层次的参考程序。
- 10 档独立子任务，共 100 分；45 组私有测试，2 组满规模下发大样例，另有题面样例。
- 数据生成器、输入验证器、输出检查器、独立快照校验器、对拍、错误程序测试、本地评分、打包及最终审计工具。
- [数据参数和哈希](../manifest.json)、[数据设计](data_design.md)、[分组配置](../config/subtasks.json)。

正式测试和下发大样例共 {manifest['operations']:,} 次操作，输入和答案合计约 {total_bytes / 2**20:.2f} MiB。

## 部分分

各档独立计分，组内全过得分，没有组间依赖。

| 子任务 | 分值 | 文件数 | 额外限制 |
| --- | ---: | ---: | --- |
{chr(10).join(subtask_rows)}

第 9 档采用短区间条件，有独立线性算法；没有沿用仅限制操作次数的早期方案。

## 正确性与工具核验

- 完整 std 在 {differential['cases']} 组测试、{differential['operations']:,} 次操作中，与全树重算及局部重算解一致；其中 {differential['brute_operations']:,} 次还与直接枚举全部存储集合核对。
- 生成器自测 {generator['cases']} 组，通过全部形态/操作/费用组合、线性深度及种子复现检查。
- 验证器、检查器和独立参考程序共通过 {tools['checks']} 项测试，包含 {tools['differential_operations']:,} 次参考解对拍输出。
- 47 组正式数据及大样例全部通过所属子任务验证与文件哈希核验。独立快照解不使用 std 的 HLD 矩阵：它用扫描线与 Fenwick 树计算包含次数，再重建规范需求并全树重算。
- 每个正式文件检查前 8 次、均匀选取的 20 个时点及末次，去重后通常为 28 次；{full_checked} 个文件还用适用的独立参考程序逐行核对。
- {mutations['mutants']} 种故意错误的实现全部被针对性测试和真实私有数据抓住，包括方案数恒 1、漏加平局、布尔引用次数、方向混淆、端点未上升、根需求遗漏、矩阵乘法计数错误、32 位截断、点修改漏下推和反向传播。

详见 [整程序对拍](differential.json)、[工具测试](judge_tools.json)、[大数据验证](data_verification.json)、[错误实现测试](mutations.json)。

## 本地实测分数

| 程序 | 算法 | 得分 | 通过的子任务 |
| --- | --- | ---: | --- |
{chr(10).join(score_rows)}

部分分程序通过 `tools/judge.py` 按 10 秒时限实际运行；暴力和短区间专用解在不支持的输入上会明确退出。std 的记录来自对所有正式文件的完整运行，以及随后独立答案核验。内存没有由本地评分脚本强制限制。

## 性能与建议限制

- 建议限制：**10 秒，256 MiB，C++17**。
- 5 组代表性用例各重复 3 次；最慢一组的中位数为 **{max_median:.3f} 秒**。
- 采样得到的最大 Windows 进程峰值工作集约 **{peak / 2**20:.2f} MiB**。
- 测试环境：`{benchmark['platform']}`，`{benchmark['compiler']}`。

这些是本机实际记录。目标评测环境的处理器、编译器及内存统计方式可能不同，正式比赛部署时应复测。完整明细见 [性能报告](benchmark.json)。

另将 std 改为“每次链修改后立即向根传播”的正确变体：它通过 24 组、7200 次小规模对拍，但在 `{probe['results'][0]['id']}` 和 `{probe['results'][1]['id']}` 上超过 10 秒；在第三组用例上仍能通过。此测试支持批量传播的实际价值，不代表所有多一层对数的实现都会超时。详见 [传播方式比较](unbatched_performance.json)。

## 打包与复现

出题人包保留全部源码、数据和报告。选手包采用文件白名单，只包含题面、题面样例和两个大样例；大样例放在选手包的 `samples/large/` 中。

完整命令见 [README](../README.md)。固定种子可以重新生成输入，答案使用统一 LF 换行；`FILE_SHA256SUMS.txt` 用于核对出题人文件，压缩包另附 SHA-256。

本报告对应已经完成的本机核验；尚未接入某个指定评测平台的导入流程。
"""
    (ROOT / "reports/acceptance.md").write_text(summary, encoding="utf-8")
    print("reports/acceptance.md generated")


if __name__ == "__main__":
    main()
