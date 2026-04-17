= 题解：传递信息 (info)

== 题目大意
在 $d$ 分钟内有 $n$ 条消息。每条消息 $i$ 有开始时间 $a_i$、持续时长 $b_i$ 和所在行数 $c_i$。
已知每一分钟老师所在的行数 $w_t$。
如果在消息持续的时间段 $[a_i, a_i + b_i - 1]$ 内，老师有任何一分钟出现在第 $c_i$ 排，消息就失效。
求成功发出的消息数量。

== 解题思路

=== 20% - 40% 数据：观察与模拟
对于这部分数据，$n$ 非常小。我们只需要针对每一条消息，用一个循环检查它持续的每一分钟即可。
- 遍历消息 $i$：
  - 遍历时间 $t$ 从 $a_i$ 到 $a_i + b_i - 1$：
    - 如果 $w_t == c_i$，标记这条消息失败。
  - 如果从未被标记失败，计数器加 1。

=== 100% 数据：高效模拟
虽然 $n=1000$，$d=10000$ 看起来不算太大，但直接暴力检查的复杂度是 $O(n times d)$。
在最坏情况下，运算次数约为 $10^7$，这在 C++ 中是完全可以接受的（通常 $10^8$ 为限）。
我们只需要注意数组下标：
- 老师的位置 $w_i$ 建议用数组存储，下标从 $1$ 到 $d$。
- 消息的判断逻辑要严格遵守题目给出的区间 $[a_i, a_i + b_i - 1]$。

== 复杂度分析
- *时间复杂度*：$O(n times "avg"(b_i))$，最坏情况下为 $O(n times d)$。
- *空间复杂度*：$O(d)$，用于存储老师每分钟的位置。

== 参考代码 (C++20)

```cpp
#include <iostream>
#include <vector>

using namespace std;

int main() {
    // 优化输入输出
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, d;
    if (!(cin >> n >> d)) return 0;

    // 存储消息数据
    struct Message {
        int a, b, c;
    };
    vector<Message> msgs(n);
    for (int i = 0; i < n; ++i) {
        cin >> msgs[i].a >> msgs[i].b >> msgs[i].c;
    }

    // 存储老师的位置，w[i] 表示第 i 分钟老师在哪一排
    vector<int> w(d + 1);
    for (int i = 1; i <= d; ++i) {
        cin >> w[i];
    }

    int success_count = 0;

    for (const auto& msg : msgs) {
        bool caught = false;
        int start_time = msg.a;
        int end_time = msg.a + msg.b - 1;

        // 检查消息持续的每一分钟
        for (int t = start_time; t <= end_time; ++t) {
            if (w[t] == msg.c) {
                caught = true;
                break; // 只要有一分钟被发现，就不用再看了
            }
        }

        if (!caught) {
            success_count++;
        }
    }

    cout << success_count << endl;

    return 0;
}
```