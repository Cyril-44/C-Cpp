#!/usr/bin/env python3
"""Create the human-readable acceptance summary and data-design index."""
import json
from pathlib import Path

from common import ROOT


def read_json(path):
    return json.loads((ROOT / path).read_text(encoding='utf-8'))


def main():
    settings = read_json('config/subtasks.json')
    cases = read_json('manifest.json')['cases']
    verification = read_json('reports/verification.json')
    mutation = read_json('reports/mutation.json')
    benchmark = read_json('reports/benchmark.json')
    judge_tools = read_json('reports/judge_tools_report.json')
    snapshots = read_json('reports/snapshot_report.json')
    private = [c for c in cases if c['kind'] == 'private']
    public = [c for c in cases if c['kind'] == 'large_sample']
    total_bytes = sum(c['input_bytes'] + c['answer_bytes'] for c in cases)
    groups_table = ['| 子任务 | 分值 | 额外约束 | 测试点数 |', '| --- | ---: | --- | ---: |']
    for group in settings['subtasks']:
        groups_table.append(f"| {group['id']} | {group['score']} | {group['description']} | "
                            f"{sum(c['subtask'] == group['id'] for c in private)} |")
    scores = []
    for name, label in [('std', '完整标程'), ('brute', '枚举全部存储集合（仅 n≤8）'),
                        ('recompute', '每次完整重算 DP'), ('disjoint', '需求互不相交的可加解法')]:
        path = ROOT / 'reports' / (name + '_score.json')
        if path.exists():
            report = json.loads(path.read_text(encoding='utf-8'))
            scores.append((label, report['score'], ', '.join(str(g['id']) for g in report['subtasks'] if g['score'])))
    score_table = ['| 程序 | 实测得分 | 通过的子任务 |', '| --- | ---: | --- |']
    score_table += [f'| {label} | {score} | {ids} |' for label, score, ids in scores]
    max_median = max(c['median_seconds'] for c in benchmark['cases'])
    memories = [c['peak_working_set_bytes'] for c in benchmark['cases'] if c['peak_working_set_bytes']]
    memory_text = f'{max(memories)/(1024*1024):.2f} MiB' if memories else '本平台未采集'
    full_checks = sum('full_recompute' in c['oracles'] for c in verification['case_checks'])
    special_checks = sum(any(name in c['oracles'] for name in ('short', 'whole_singleton'))
                         for c in verification['case_checks'])
    acceptance = f'''# 初稿 v1 验收摘要

日期：2026-10-20。题目：《空间相同，方案不同》。

## 已交付内容

- 正式中文题面与完整题解，题解包含全部 10 档部分分做法。
- C++17 标程、独立枚举暴力、每次全树重算程序及三个结构性部分分程序。
- 固定种子生成器、支持 10 档约束的输入校验器、答案检查器、本地子任务评分器。
- **{len(private)} 个正式测试点**，共 {sum(c['q'] for c in private):,} 次操作，每点附标准答案。
- 题面样例与 **{len(public)} 组下发大样例**，规模分别为 `100000 / 300000`、`99999 / 200000`（n / q）。
- 数据文件 SHA-256、可复现生成配置、评测工具自测、正确性核验、性能和错误程序检出报告。
- 独立的主办方 ZIP 与选手附件 ZIP；下发包不含题解、程序及正式评测数据。

输入与答案文本总量约 **{total_bytes/(1024*1024):.2f} MiB**，未计文档与源代码。

## 部分分初稿

各组独立、组内全过得分，总计 100 分。

{chr(10).join(groups_table)}

前三档提供边界处理、真正的穷举、静态树 DP 的得分路径。第 4—7 档分别对应可加性、最近公共祖先公式和小组件计数；第 8—9 档允许暂不解决删除共享需求或一般区间分解。完整算法负责最后 25 分。

## 已执行的验证

1. 全部 {len(cases)} 份正式输入及大样例通过输入合法性检查；正式输入另通过所属子任务的额外约束检查。
2. 新标程与直接枚举所有存储集合的暴力核对 **{verification['brute_compared_operations']:,} 次操作**。其中 n=1..5 覆盖全部需求集合，n=6..8 使用多组固定种子随机增删。
3. 全部 {len(cases)} 份数据均在前 16 次、均匀分布的约 40 个检查点及末次操作，用独立的全树重算程序核对答案。**大规模一般数据采用检查点核对，不宣称逐行暴力验证。**
4. 其中 {full_checks} 份小规模数据逐行用全树重算核对；第 6、7 档共 {special_checks} 份数据另用独立公式/组件算法逐行核对全部答案。
5. 校验器与 checker 的 **{judge_tools['tests']} 项正反测试**通过，快照参考程序的 **{snapshots['tests']} 项测试**通过。
6. **{mutation['killed']} 种常见错误程序全部被正式数据检出**，包括需求次数误用布尔值、忽略平局方案、32 位乘法溢出、错误奇数分裂及空集计数错误。
7. 本地评分器按题面 10 组运行，结果如下。慢程序的 TLE 属于预期分档结果。

{chr(10).join(score_table)}

## 时空限制初稿

- 建议 3 秒、256 MiB。
- 本机代表性最大数据三次运行的最大中位数为 **{max_median:.3f} 秒**；采样得到的最大进程峰值工作集为 **{memory_text}**。
- 正确的每次全树重算程序在一般最大数据 `10_01` 上触发 3 秒时限：**{'是' if benchmark['recompute_10_01_timed_out_at_3_seconds'] else '否'}**。
- 本机环境：`{benchmark['platform']}`；编译器：`{benchmark['compiler']}`。

这些是本机实测值，包含进程启动时间。正式发布前仍需在目标评测机复测时限、语言倍率和内存限制；不依靠卡掉 O(q log² n) 实现来区分满分解。

## 验收入口

- [题面](../statement.md)
- [题解](../editorial.md)
- [数据设计与测试点清单](data_design.md)
- [标程](../solutions/std.cpp)
- [生成配置](../config/cases.json)
- [评分配置](../config/subtasks.json)
- [正确性核验原始报告](verification.json)
- [错误程序检出报告](mutation.json)
- [性能原始报告](benchmark.json)

这版采用通用题包格式；目标比赛平台、最终时限及语言倍率尚未指定。题目本身、所有数据和工具已完成，可据此验收并提出修改。
'''
    (ROOT / 'reports/acceptance.md').write_text(acceptance, encoding='utf-8')

    design = ['# 数据设计与测试点索引', '',
              '生成参数见 `config/cases.json`；生成器版本以本题包 `tools/gen.py` 为准。每个文件的大小和 SHA-256 见 `manifest.json`。', '',
              '## 覆盖目标', '',
              '- 空需求与单点：检查 `0 1`、叶子状态，以及大量重复加入/删除的合法循环。',
              '- 小规模 Gray-code：遍历需求集合，精确暴露方案数和包含关系的错误。',
              '- 共享分解节点：前缀、后缀和长区间交叠后删除其中一部分，检查引用次数。',
              '- 大量独立平局：互不相交的双叶组件产生乘积形式的方案数，覆盖取模及 64 位乘法。',
              '- 任意奇数树形：使用 99999、99991、32769、7 等，检查中点和兄弟关系。',
              '- 前缀波次、层级插入及随机删改：覆盖深层状态改变而祖先需求仍然存在的情形。',
              '- 双叶与跨兄弟短区间混合：第 7 档直接需求次数可能来自不同原区间，不能用输入单点是否存在代替。',
              '- 第 6 档先激活整段和全部单点，再连续删除单点，使最左、最右未激活叶子的共同祖先发生变化。', '',
              '## 正式数据', '',
              '| 测试点 | 组别 | n | q | 模式 | 种子 |',
              '| --- | ---: | ---: | ---: | --- | ---: |']
    for c in private:
        design.append(f"| {c['id']} | {c['subtask']} | {c['n']} | {c['q']} | `{c['mode']}` | {c['seed']} |")
    design += ['', '## 下发大样例', '', '| 文件 | n | q | 模式 | 种子 |', '| --- | ---: | ---: | --- | ---: |']
    for c in public:
        design.append(f"| {c['id']} | {c['n']} | {c['q']} | `{c['mode']}` | {c['seed']} |")
    design += ['', '大样例使用单独的种子，未与任何正式测试点共享完全相同的输入文件。它们不参与计分。', '',
               '## 错误程序检出', '', '| 错误类型 | 首个检出测试点 |', '| --- | --- |']
    for entry in mutation['mutants']:
        design.append(f"| `{entry['mutant']}` | {entry['case']} |")
    (ROOT / 'reports/data_design.md').write_text('\n'.join(design) + '\n', encoding='utf-8')
    print('Wrote reports/acceptance.md and reports/data_design.md')


if __name__ == '__main__':
    main()
