# 《空间相同，方案不同》选手附件

本附件包含题面、题面样例，以及两组用于本地调试的大样例。

| 文件 | 规模 |
| --- | --- |
| `samples/sample1.in` / `samples/sample1.ans` | 题面样例，n=2，q=6 |
| `samples/large/large1.in` / `samples/large/large1.ans` | n=100000，q=300000 |
| `samples/large/large2.in` / `samples/large/large2.ans` | n=99999，q=200000 |

`.in` 为输入，`.ans` 为正确输出。程序每次读取一份输入，通过标准输出回答每次操作。

例如，编译后可将 `samples/large/large1.in` 重定向到程序的标准输入，再将输出与对应的 `.ans` 按整数比较。Windows 与其他系统的换行差异不影响答案。

建议评测限制为 3 秒、256 MiB；最终以比赛平台公布的设置为准。通过大样例不代表能通过全部正式数据。
