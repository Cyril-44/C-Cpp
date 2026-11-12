#include <bits/stdc++.h>
constexpr int N = 300005;
#ifdef CLANGD
#define N 35
#endif
std::vector<int> g[N];
int edgs[N][2], fa[N], down[N], up[N], cup[N];
void dfs(int u) {
    for (int v : g[u]) if (v != fa[u])
        fa[v] = u, dfs(v);
}
int n;
struct BIT {
    int64_t tr[N];
    int val[N];
    void upd(int p, int x) { for (; p <= n; p += p & -p) tr[p] += x; }
    void modify(int p, int x) {
        upd(p, x - val[p]);
        val[p] = x;
    }
    int64_t sum(int p) const {
        int64_t res = 0;
        for (; p > 0; p -= p & -p) res += tr[p];
        return res;
    }
    int64_t inquire(int l, int r) const { return sum(r) - sum(l-1); }
} fs;
inline void unite(int i) {
    const int u = edgs[i][0], v = edgs[i][1];
    up[v] = up[u] + down[u] - cup[v];
    down[u] += -cup[v] + down[v], cup[v] = down[v];
    fs.modify(u, down[u] + up[u]);
    fs.modify(v, down[v] + up[v]);
}
int main() {
    freopen("bit.in", "r", stdin);
    freopen("bit.out", "w", stdout);
    int q;
    scanf("%d%d", &n, &q);
    for (int i = 1; i < n; i++) {
        int u, v;
        scanf("%d%d", &u, &v);
        edgs[i][0] = u, edgs[i][1] = v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs(1);
    for (int i = 1; i <= n; i++) down[i] = 1, fs.modify(i, 1);
    for (int i = 1; i < n; i++)
        if (fa[edgs[i][0]] == edgs[i][1]) std::swap(edgs[i][0], edgs[i][1]);
    for (int op, l, r; q--; ) {
        scanf("%d%d", &op, &l);
        if (op == 1) {
            scanf("%d", &r);
            printf("%lld\n", fs.inquire(l, r));
        } else {
            unite(l);
        }
    }
}