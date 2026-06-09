#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#include <bits/stdc++.h>
using namespace std;
// k=2，修改差分数组的正负
// 对排名串哈希（对相对大小hash）
// 不能重合
// 可以带修莫队
// 区间数颜色，每次会修改最多2k个位置的hash值
// 每加入/删除一个颜色先贡献区间内该颜色总数，然后减去邻近2(k-1)个位置和其相同的颜色个数
namespace FAST_IO {
const int BUF(1 << 20);
char buf[BUF], *p1 = buf, *p2 = buf;
char pbuf[BUF], *p = pbuf;
char gc() {
    return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, BUF, stdin), p1 == p2) ? EOF : *p1++;
}
void pc(char c) {
    *p++ = c;
    if (p - pbuf == BUF) fwrite(pbuf, 1, BUF, stdout), p = pbuf;
}
void flush() {
    fwrite(pbuf, 1, p - pbuf, stdout);
    p = pbuf;
}
template <typename T> void read(T &x) {
    x = 0;
    static char c;
    T f = 1;
    do {
        c = gc();
        if (c == '-') f = -f;
    } while (!isdigit(c));
    do {
        x = (x << 3) + (x << 1) + (c ^ 48);
        c = gc();
    } while (isdigit(c));
    x *= f;
}
template <typename T, typename... Args> void read(T &x, Args &...args) {
    read(x);
    read(args...);
}
template <typename T> void write(T x) {
    if (x < 0) {
        pc('-');
        x = -x;
    }
    static char stk[1 << 8], *tp;
    tp = stk;
    do *tp++ = (x % 10) ^ 48;
    while (x /= 10);
    while (tp != stk) pc(*--tp);
}
void write(char c) {
    pc(c);
}
template <typename T, typename... Args> void write(T x, Args... args) {
    write(x);
    write(args...);
}
struct TMP {
    ~TMP() { flush(); }
} tmp;
}; // namespace FAST_IO
using FAST_IO::flush;
using FAST_IO::read;
using FAST_IO::write;
typedef long long ll;
const int N = 1e5 + 10, M = 3628800;
int n, m, m2, k, p[N], B, a[N], h[11], g[11], fac[11], c[M], tmp[N];
bool vis[11];
ll ans[N], res;
struct node {
    int l, r, ti, id;
} q[N];
struct chg {
    int p, c;
} b[N * 23];
bool cmp(node x, node y) {
    return x.l / B == y.l / B ? (x.r / B == y.r / B ? x.ti < y.ti : x.r < y.r) : x.l < y.l;
}
inline int getid() {
    memset(vis, 0, sizeof(vis));
    int res = 0;
    for (int i = 1; i <= k; i++) {
        int c1 = 0;
        for (int j = 1; j < g[i]; j++)
            if (!vis[j]) c1++;
        res += c1 * fac[k - i];
        vis[g[i]] = 1;
    }
    return res;
}
inline void opt(int x) { // 重新计算x位置的相对大小hash
    for (int i = 1; i <= k; i++) h[i] = p[x + i - 1];
    sort(h + 1, h + k + 1);
    for (int i = 1; i <= k; i++) g[i] = lower_bound(h + 1, h + k + 1, p[x + i - 1]) - h;
    int p = getid();
    if (tmp[x] != p) b[++m2] = {x, p}, tmp[x] = p;
}
inline void add(int l, int r, int x, int y, int o) {
    if (o < 0) c[y]--;
    int s = c[y] - count(a + max(l, x - k + 1), a + min(x + k - 1, r) + 1, y) + (a[x] == y);
    res += o * s;
    if (o > 0) c[y]++;
}
int main() {
    freopen("stars.in", "r", stdin);
    freopen("stars.out", "w", stdout);
    fac[0] = 1;
    for (int i = 1; i < 10; i++) fac[i] = fac[i - 1] * i;
    read(n, m, k);
    for (int i = 1; i <= n; i++) read(p[i]);
    for (int i = 1; i <= n - k + 1; i++) opt(i);
    int _m = m, mt = 0;
    m = 0;
    for (int t = 1; t <= _m; t++) {
        int op;
        read(op);
        if (op == 1) {
            int x, y;
            read(x, y);
            if (x > y) swap(x, y);
            swap(p[x], p[y]);
            for (int i = x; i >= x - k + 1; i--) opt(i);
            for (int i = y; i >= y - k + 1 && i > x; i--) opt(i);
        } else {
            int l, r;
            read(l, r);
            r = r - k + 1;
            mt++;
            if (l > r) continue;
            q[++m] = {l, r, m2, mt};
        }
    }
    if (!m2)
        B = sqrt(n);
    else
        B = pow(m2, 2.0 / 3.0);
    sort(q + 1, q + m + 1, cmp);
    for (int l = 1, r = 0, lt = 0, i = 1; i <= m; i++) {
        while (l > q[i].l) --l, add(l, r, l, a[l], 1);
        while (r < q[i].r) ++r, add(l, r, r, a[r], 1);
        while (l < q[i].l) add(l, r, l, a[l], -1), l++;
        while (r > q[i].r) add(l, r, r, a[r], -1), r--;
        while (lt < q[i].ti) {
            lt++;
            if (l <= b[lt].p && b[lt].p <= r) {
                add(l, r, b[lt].p, b[lt].c, 1);
                add(l, r, b[lt].p, a[b[lt].p], -1);
            }
            swap(a[b[lt].p], b[lt].c);
        }
        while (lt > q[i].ti) {
            if (l <= b[lt].p && b[lt].p <= r) {
                add(l, r, b[lt].p, a[b[lt].p], -1);
                add(l, r, b[lt].p, b[lt].c, 1);
            }
            swap(a[b[lt].p], b[lt].c);
            lt--;
        }
        ans[q[i].id] = res;
    }
    for (int i = 1; i <= mt; i++) write(ans[i], '\n');
}