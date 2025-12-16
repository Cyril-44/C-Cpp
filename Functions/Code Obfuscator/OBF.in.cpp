#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
template <class T> void in(T &x) {
    x = 0;
    int f = 1;
    char c = getchar();
    while (c < '0' || c > '9') {
        if (c == '-') f = -1;
        c = getchar();
    }
    while (c >= '0' && c <= '9') {
        x = x * 10 + c - '0';
        c = getchar();
    }
}
const int N = 1000010, M = 20, P = 30;
vector<int> gr[N];
int n, m, a[N], fa, cur, d;
struct Query {
    int dep, id;
};
vector<Query> q[N];
int dfn[N], son[N], dep[N], mxdep[N], ck;
void init(int u) {
    mxdep[u] = dep[u];
    for (int v : gr[u]) {
        dep[v] = dep[u] + 1;
        init(v);
        mxdep[u] = max(mxdep[u], mxdep[v]);
        if (mxdep[son[u]] <= mxdep[v]) son[u] = v;
    }
}
ll f[N][M], g[N][P];
int cnt[N];
int c(int u, int len) {
    if (mxdep[u] - dep[u] >= len) return dfn[u] + len;
    return 0;
}
ll ans[N];
void dfs(int u) {
    dfn[u] = ++ck;
    if (son[u]) dfs(son[u]);
    for (int v : gr[u]) {
        if (v == son[u]) continue;
        dfs(v);
        int add = dep[son[u]] - dep[v];
        for (int i = 0; i <= mxdep[v] - dep[v]; i++) {
            for (int j = 0; j < M; j++) f[c(son[u], i)][j] += f[c(v, i)][j];
            for (int j = 0; j < P; j++) g[c(son[u], i)][j] += g[c(v, i)][j];
            cnt[c(son[u], i)] += cnt[c(v, i)];
        }
    }
    f[c(u, 0)][0] = 0;
    for (int j = 0; j < P; j++) g[c(u, 0)][j] = g[c(u, 1)][j] + (a[u] & (1 << j));
    cnt[c(u, 0)] = cnt[c(u, 1)] + 1;
    for (int j = 1; j < M; j++) {
        f[c(u, 0)][j] = f[c(u, 0)][j - 1] + g[c(u, 0)][j - 1] - g[c(u, 1 << j - 1)][j - 1];
        f[c(u, 0)][j] += f[c(u, 1 << j - 1)][j - 1] + (1ll << j - 1) * (cnt[c(u, 1 << j - 1)] - cnt[c(u, 1 << j)]) -
                         g[c(u, 1 << j - 1)][j - 1] + g[c(u, 1 << j)][j - 1];
    }
    for (const Query &x : q[u]) {
        int pre = 0, lst = P - 1;
        for (int i = M - 1; i >= 0; i--) {
            if (x.dep & (1 << i)) {
                ans[x.id] += f[c(u, pre)][i];
                for (; lst > i; lst--) ans[x.id] += g[c(u, pre)][lst] - g[c(u, x.dep)][lst];
                ans[x.id] += g[c(u, pre)][lst] - g[c(u, pre + (1 << lst))][lst];
                ans[x.id] += (cnt[c(u, pre + (1 << lst))] - cnt[c(u, x.dep)]) * (1ll << lst);
                ans[x.id] -= g[c(u, pre + (1 << lst))][lst] - g[c(u, x.dep)][lst];
                lst--;
                pre += (1 << i);
            }
        }
    }
}
int main() {
    in(n);
    for (int i = 1; i <= n; i++) in(a[i]);
    for (int i = 2; i <= n; i++) {
        in(fa);
        gr[fa].push_back(i);
    }
    in(m);
    for (int i = 1; i <= m; i++) {
        in(cur);
        in(d);
        d++;
        q[cur].push_back((Query){d, i});
    }
    init(1);
    dfs(1);
    for (int i = 1; i <= m; i++) printf("%lld\n", ans[i]);
    return 0;
}