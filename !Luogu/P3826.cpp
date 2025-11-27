#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 100005;
struct Node {
    int w, cnt, sub;
    bool operator< (const Node &rhs) const {
        return w < rhs.w;
    }
} a[N << 1];
int toAns[N * 10], fa[N], res[N];
int getroot(int u) {
    return u ^ fa[u] ? fa[u] = getroot(fa[u]) : u;
}
int T = 0, tcnt = 0;
long long cur, ans[N];
bool del (Node& veg) {
    if (!veg.cnt) return false;
    int t = veg.sub ? T : std::min(T, (veg.cnt-1) / veg.sub + 1);
    t = getroot(t);
    if (!t) return 0;
    --veg.cnt;
    cur += veg.w;
    --res[t];
    if (!res[t]) fa[t] = t-1;
    ++tcnt;
    ans[toAns[tcnt]] = cur;
    return true;
}
int main() {
    int n, m, k;
    scanf("%d%d%d", &n, &m, &k);
    for (int i = 1, w, w1, c, x; i <= n; i++) {
        scanf("%d%d%d%d", &w, &w1, &c, &x);
        a[i-1 << 1 | 1] = Node {w, c-1, x};
        a[i << 1] = Node {w + w1, 1, x};
    }
    std::sort(a+1, a+1 + (n<<1));
    for (int i = 1, q; i <= k; i++) {
        scanf("%d", &q);
        T = std::max(T, q);
        toAns[q * m] = i;
    }
    for (int i = 1; i <= T; i++)
        res[i] = m, fa[i] = i;
    int M = m * T;
    for (int i = 1; i <= (n << 1) && tcnt < M; i++) {
        while (del(a[i]));
    }
    for (int i = M; i > tcnt; i--)
        ans[toAns[i]] = cur;
    for (int i = 1; i <= k; i++)
        printf("%lld\n", ans[i]);
    return 0;
}