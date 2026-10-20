# 自由分割，按价存储

选手下发文件包含：

- `statement.md`：完整题面、输入输出约定和十档部分分。
- `samples/sample1.in`、`samples/sample1.ans`：题面样例。
- `samples/large/large_sample_01.in`、`samples/large/large_sample_01.ans`：第一组大样例，\(n=q=100000\)。
- `samples/large/large_sample_02.in`、`samples/large/large_sample_02.ans`：第二组大样例，\(n=q=100000\)，树高为 \(99999\)。

大样例用于检查输入输出处理、长链适应性及较大规模运行情况，不参与计分。通过这些样例不代表程序可以通过全部评测数据。

`.ans` 文件是对应输入的完整标准输出。建议逐个整数比较；行尾空白不影响答案。

本题使用标准输入与标准输出，不要求文件重定向。根为节点 \(1\)，叶子序号按从左到右的顺序确定，不能将节点编号视为叶子序号。

各子任务独立计分，组内全部测试通过才得到该组分数。
