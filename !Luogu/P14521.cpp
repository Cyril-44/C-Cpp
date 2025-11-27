#include <stdio.h>
#include <vector>
#include <algorithm>
constexpr int N = 500005, M = 1000005;
std::vector<int> g[N];
using LL = __int128;
struct Range {LL l, r;} rg[N];
struct Query {
    LL x;
    int id;
    inline bool operator< (const Query&rhs) const {
        return x < rhs.x;
    }
} q[M];
int m, ans[M], aans[M];
LL val[N];
void dfs(int u, LL l, LL r, LL pre) {
    if (l > r) return;
    // fprintf(stderr, "%d: [%lld, %lld]++\n", u, l, r);
    ans[std::lower_bound(q+1, q+1+m, Query{l,0})-q]++;
    ans[std::upper_bound(q+1, q+1+m, Query{r,0})-q]--;
    pre += val[u];
    for (int v : g[u]) {
        dfs(v, std::max(l, rg[v].l - pre), std::min(r, rg[v].r - pre), pre);
    }
}
int main() {
    int n;
    scanf("%d%d", &n, &m);
    for (int i = 2, fa; i <= n; i++) {
        long long l, r;
        scanf("%d%lld%lld", &fa, &l, &r);
        rg[i] = {l, r};
        g[fa].push_back(i);
    }
    char op;
    for (int i = 1; i <= n; i++) {
        long long v;
        scanf(" %c %lld", &op, &v);
        val[i] = v;
        if (op == '-') val[i] = -val[i];
    }
    for (int i = 1; i <= m; i++) {
        long long x;
        scanf("%lld", &x);
        q[i] = {x, i};
    }
    std::sort(q+1, q+1 + m);
    dfs(1, (LL)-3e24, (LL)3e24, 0);
    for (int i = 1; i <= m; i++) {
        ans[i] += ans[i-1];
        aans[q[i].id] = ans[i];
    }
    for (int i = 1; i <= m; i++)
        printf("%d\n", aans[i]);
    return 0;
}