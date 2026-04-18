= 巨大 DS 题解
<巨大-ds-题解>
这道题目是一个经典的数据结构问题。由于给定的序列是一个
#strong[排列]（$1 tilde.op n$
每个数恰好出现一次），我们可以利用这个性质将问题转化。

== 题目大意
<题目大意>
在数组 $a$ 的下标范围 $\[l\,r\]$ 内，寻找从数值 $k$ 开始的连续整数。

#strong[注意：]
"连续数"指的是数值上的连续（$k\,k + 1\,k + 2\,dots.h$），而不是下标上的连续。

例如：序列为 $\[1\,3\,2\,4\,5\]$，在区间 $\[1\,3\]$ 中，$k = 1$
的连续数有 $1\,2\,3$，所以答案是 $3$。

== 解题思路与部分分说明
<解题思路与部分分说明>
=== Subtask 1 & 2：暴力模拟 ($n\,q lt.eq 3000$)
<subtask-1-2暴力模拟-n-q-le-3000>
对于每一组询问 $\(l\,r\,k\)$：

+ 我们先用一个布尔数组（或标记数组）记录下 $a\[l dots.h r\]$
  中出现了哪些数字。

+ 从数值 $k$ 开始，检查 $k$ 是否出现，$k + 1$ 是否出现…
  直到某个数字没出现为止。

+ #strong[复杂度]：$O\(q times n\)$。对于 $3000$ 的数据量，计算量约
  $9 times 10^6$，可以稳过。

=== Subtask 3：特殊性质 $k = 1$
<subtask-3特殊性质-k1>
当 $k$ 固定为 $1$ 时，我们要找的是 $\[l\,r\]$ 中 $1\,2\,3 dots.h$
最长连续到几。

+ 我们可以预处理每个数值 $i$ 在原数组中的位置 $p o s\[i\]$。

+ 询问变成了：找到最大的 $x$，使得
  $p o s\[1\]\,p o s\[2\]\,dots.h\,p o s\[x\]$ 全都在 $\[l\,r\]$ 之间。

+ 这等价于：$min\(p o s\[1 dots.h x\]\)gt.eq l$ 且
  $max\(p o s\[1 dots.h x\]\)gt.eq r$。

+ 可以通过二分答案 $x$，配合 #strong[ST 表] 查询位置的最大最小值来解决。

+ #strong[复杂度]：$O\(n log n + q log n\)$。

=== Subtask 4：满分做法 (100% 数据)
<subtask-4满分做法-100-数据>
对于 $10^6$ 的数据，我们需要更高效的处理方式。

#strong[核心转化：]

由于 $a$ 是排列，数值 $v$ 在区间 $\[l\,r\]$ 中出现，当且仅当它的出现位置
$p o s\[v\]in\[l\,r\]$。

我们要找最大的 $x$，使得：

$ forall v in\[k\,k + x - 1\]\,quad l lt.eq p o s\[v\]lt.eq r $

这可以拆解为两个约束条件：

+ $min_(i = k)^(k + x - 1) { p o s\[i\]} gt.eq l$

+ $max_(i = k)^(k + x - 1) { p o s\[i\]} lt.eq r$

#strong[算法步骤：]

+ #strong[预处理位置]：记录每个数的位置 $p o s\[a_i\]= i$。

+ #strong[建立 ST 表]：对 $p o s$ 数组建立两棵 ST
  表，分别维护区间最大值和区间最小值。

+ #strong[二分答案]：

  对于每个询问 $\(l\,r\,k\)$，二分连续数的个数 $m i d$。

  - 检查数值区间 $\[k\,k + m i d - 1\]$ 对应的位置范围。

  - 利用 ST 表 $O\(1\)$ 查询该数值区间的 $p o s$ 最小值是否 $gt.eq l$
    且最大值是否 $lt.eq r$。

+ #strong[复杂度分析]：

  - 预处理 ST 表：$O\(n log n\)$。

  - 单次查询：$O\(log n\)$。

  - 总复杂度：$O\(n log n + q log n\)$。在 $10^6$
    级别下，配合快速输入输出即可通过。

== 参考代码 (C++20)
<参考代码-c20>
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 1000005;
const int LOGN = 21;

int pos[MAXN];
int st_max[MAXN][LOGN], st_min[MAXN][LOGN];
int lg2[MAXN];

// 预处理 log 函数
void precompute_logs(int n) {
    lg2[1] = 0;
    for (int i = 2; i <= n; i++)
        lg2[i] = lg2[i / 2] + 1;
}

// 查询数值区间 [L, R] 对应位置的最小值和最大值
pair<int, int> query(int L, int R) {
    int k = lg2[R - L + 1];
    int mn = min(st_min[L][k], st_min[R - (1 << k) + 1][k]);
    int mx = max(st_max[L][k], st_max[R - (1 << k) + 1][k]);
    return {mn, mx};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, q;
    cin >> n >> q;

    for (int i = 1; i <= n; i++) {
        int val;
        cin >> val;
        pos[val] = i; // 记录数值 val 出现的位置
    }

    // 初始化 ST 表
    for (int i = 1; i <= n; i++) {
        st_max[i][0] = st_min[i][0] = pos[i];
    }

    for (int j = 1; j < LOGN; j++) {
        for (int i = 1; i + (1 << j) - 1 <= n; i++) {
            st_max[i][j] = max(st_max[i][j - 1], st_max[i + (1 << (j - 1))][j - 1]);
            st_min[i][j] = min(st_min[i][j - 1], st_min[i + (1 << (j - 1))][j - 1]);
        }
    }

    precompute_logs(n);

    while (q--) {
        int l, r, k;
        cin >> l >> r >> k;

        // 如果 k 本身就不在区间内，直接输出 0
        if (k > n || pos[k] < l || pos[k] > r) {
            cout << 0 << "\n";
            continue;
        }

        // 二分连续数的个数
        int L = 1, R = n - k + 1, ans = 1;
        while (L <= R) {
            int mid = L + (R - L) / 2;
            auto [p_min, p_max] = query(k, k + mid - 1);
            if (p_min >= l && p_max <= r) {
                ans = mid;
                L = mid + 1;
            } else {
                R = mid - 1;
            }
        }
        cout << ans << "\n";
    }


    return 0;
}
```

=== 总结
<总结>
这道题的关键在于抓住 #strong["排列"] 和 #strong["数值连续"]
两个点。通过将数值看作索引，位置看作值，问题就从复杂的搜索转化为了简单的区间最值查询问题。
