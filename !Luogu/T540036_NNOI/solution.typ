= 题解：吵闹班级 (noise)

== 题目分析

题目要求我们将 $n$ 个同学分成 $k$ 组，每组人数必须相等（即 $n/k$ 是整数）。
这意味着 $k$ 必须是 $n$ 的*约数*。

*计算公式：*
1. 每组人数 $m = n/k$。
2. 设第 $j$ 组的最大值为 $M_j$。
3. 总吵闹值 $S = (sum_{j=1}^k M_j) times m$。

我们的目标是找到一个 $k$（$k$ 是 $n$ 的约数），使得 $S$ 最小。



== 解题思路

=== 1. 暴力档 (10% - 30% 数据)
对于 $n <= 1000$ 的情况，我们可以枚举 $n$ 的所有约数 $k$。
对于每个 $k$，计算出 $k$ 组的最大值之和，最后乘以 $n/k$。

- *复杂度：* 约数个数 $times n$。对于 $10^5$ 以上的数据会超时。

=== 2. 满分优化 (100% 数据)
观察公式 $S = (sum_{j=1}^k M_j) times (n/k)$。
我们可以转换视角：不再枚举组数 $k$，而是直接枚举**每组的人数 $m$**。
只要 $m$ 是 $n$ 的约数，对应的组数就是 $k = n/m$。

*关键点：*
$n$ 高达 $5 times 10^6$，我们必须在线性或接近线性的时间内完成计算。
1. 预处理 $n$ 的所有约数（或者直接从 $1$ 到 $n$ 枚举，如果是约数再计算）。
2. 为了快速求区间最大值，由于是固定长度的分段，我们可以直接遍历数组求最大。

*注意：* $n$ 的最大值较大，建议使用快速读入。



== 示例代码 (C++)

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// 快速读入，应对 5*10^6 的数据量
inline int read() {
    int x = 0; char ch = getchar();
    while (ch < '0' || ch > '9') ch = getchar();
    while (ch >= '0' && ch <= '9') {
        x = x * 10 + ch - '0';
        ch = getchar();
    }
    return x;
}

const int MAXN = 5000005;
int a[MAXN];
int n;

long long solve(int m) {
    long long sum_max = 0;
    int k = n / m;
    for (int i = 0; i < k; i++) {
        int cur_max = 0;
        // 遍历第 i 组的所有人，求最大值
        for (int j = i * m + 1; j <= (i + 1) * m; j++) {
            if (a[j] > cur_max) cur_max = a[j];
        }
        sum_max += cur_max;
    }
    return sum_max * m;
}

int main() {
    n = read();
    for (int i = 1; i <= n; i++) a[i] = read();

    long long min_noise = -1; // 初始化为一个极大值

    // 枚举每组的人数 m，m 必须是 n 的约数
    for (int m = 1; m <= n; m++) {
        if (n % m == 0) {
            long long res = solve(m);
            if (min_noise == -1 || res < min_noise) {
                min_noise = res;
            }
        }
    }

    printf("%lld\n", min_noise);
    return 0;
}
```



== 复杂度温馨提示
虽然枚举约数并遍历数组看起来比较慢，但实际上对于一个数 $n$，所有约数 $m$ 对应的 $n/m$ 之和（即计算次数）并不会爆炸。
- 当 $n=5 times 10^6$ 时，此算法在时限内可以通过。
- *技巧：* 如果 $a_i$ 全为 $0$，结果显然是 $0$。
