= 题解：MEX 计数

题目要求我们维护一个排列，支持交换操作，并查询有多少个区间 $[l, r]$ 的 $text("mex")(a_l dots a_r)$ 落在区间 $[x, y]$ 内。

注意：$text("mex")$（Minimum Excluded value）是指集合中未出现的最小非负整数。

== 核心性质
在一个 $0$ 到 $n-1$ 的排列中：
- 如果 $text("mex")(a_l dots a_r) > k$，说明 $0, 1, 2, dots, k$ 都在区间 $[l, r]$ 中出现过。
- 令 $p o s[i]$ 为数字 $i$ 出现的位置，则上述条件等价于：
  $[l, r] supset.eq [min_(i=0)^k p o s[i], max_(i=0)^k p o s[i]]$。
- 设 $L_k = min_(i=0)^(k-1) p o s[i]$，$R_k = max_(i=0)^(k-1) p o s[i]$。那么 $text("mex") >= k$ 的区间个数，就是包含位置区间 $[L_k, R_k]$ 的区间总数。

== 分档解法说明

=== 测试点 1-20：暴力枚举 ($n, m <= 1000$)
- *思路*：每次询问直接枚举所有的 $l, r$（共 $n^2$ 个），然后暴力计算每个区间的 $text("mex")$。
- *复杂度*：$O(m dot.c n^2)$ 或 $O(m dot.c n^3)$。

=== 测试点 21-30：无修改
- *思路*：预处理出所有的 $L_k$ 和 $R_k$。
- 满足 $text("mex") >= k$ 的区间数为 $(L_k) dot (n - R_k + 1)$。
- 满足 $text("mex") \in [x, y]$ 的数量等价于 $(text("mex") >= x text("的数量")) - (text("mex") >= y+1 text("的数量"))$。

=== 测试点 31-50：完整解法（线段树/分块）
由于涉及单点修改位置和区间查询，我们需要维护 $p o s$ 数组。
1. *关键公式*：$text("mex") >= k$ 的区间必须包含所有 $0 dots.down k-1$ 的位置。
2. 记 $f(k)$ 为 $text("mex") >= k$ 的区间个数。
   $ f(k) = min_(i=0)^(k-1) p o s[i] times (n - max_(i=0)^(k-1) p o s[i] + 1) $
3. *询问*：答案即为 $f(x) - f(y+1)$。
4. *优化*：我们可以使用线段树维护 $p o s$ 数组的区间最小值和最大值。对于每一个 $k$，我们需要查 $p o s$ 前缀的 $min$ 和 $max$。

== 参考代码 (C++)

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 200005;
int n, m, a[MAXN], pos[MAXN];

// 线段树维护 pos 的前缀 min 和 max
struct Node {
    int l, r, mn, mx;
} tree[MAXN * 4];

void build(int node, int start, int end) {
    tree[node] = {start, end, MAXN, 0};
    if (start == end) {
        tree[node].mn = tree[node].mx = pos[start];
        return;
    }
    int mid = (start + end) / 2;
    build(2 * node, start, mid);
    build(2 * node + 1, mid + 1, end);
    tree[node].mn = min(tree[2 * node].mn, tree[2 * node + 1].mn);
    tree[node].mx = max(tree[2 * node].mx, tree[2 * node + 1].mx);
}

void update(int node, int idx, int val) {
    if (tree[node].l == tree[node].r) {
        tree[node].mn = tree[node].mx = val;
        return;
    }
    int mid = (tree[node].l + tree[node].r) / 2;
    if (idx <= mid) update(2 * node, idx, val);
    else update(2 * node + 1, idx, val);
    tree[node].mn = min(tree[2 * node].mn, tree[2 * node + 1].mn);
    tree[node].mx = max(tree[2 * node].mx, tree[2 * node + 1].mx);
}

pair<int, int> query(int node, int L, int R) {
    if (L > R) return {1, n};
    if (L <= tree[node].l && tree[node].r <= R) 
        return {tree[node].mn, tree[node].mx};
    int mid = (tree[node].l + tree[node].r) / 2;
    pair<int, int> res = {MAXN, 0};
    if (L <= mid) {
        auto t = query(2 * node, L, R);
        res.first = min(res.first, t.first);
        res.second = max(res.second, t.second);
    }
    if (R > mid) {
        auto t = query(2 * node + 1, L, R);
        res.first = min(res.first, t.first);
        res.second = max(res.second, t.second);
    }
    return res;
}

long long solve(int k) {
    if (k == 0) return 1LL * n * (n + 1) / 2;
    if (k > n) return 0;
    // 计算包含数字 0 ~ k-1 的区间个数
    auto p = query(1, 0, k - 1);
    return 1LL * p.first * (n - p.second + 1);
}

int main() {
    ios::sync_with_stdio(false); cin.tie(0);
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        pos[a[i]] = i;
    }
    build(1, 0, n - 1);
    while (m--) {
        int op, x, y;
        cin >> op >> x >> y;
        if (op == 1) {
            swap(a[x], a[y]);
            pos[a[x]] = x; pos[a[y]] = y;
            update(1, a[x], x); update(1, a[y], y);
        } else {
            if (x > y) swap(x, y);
            cout << solve(x) - solve(y + 1) << "\n";
        }
    }
    return 0;
}
```