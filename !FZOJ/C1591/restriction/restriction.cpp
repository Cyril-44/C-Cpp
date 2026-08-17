#include <cstdio>
#include <vector>
#include <algorithm>
constexpr int N = 100005;
struct DSU {
    int fa[N], sz[N], hist[N];
    int top;
    void init(int n) { for (int i = 1; i <= n; i++) fa[i] = i, sz[i] = 1; }
    int find(int u) { while (u != fa[u]) u = fa[u]; return u; }
    void merge(int u, int v) {
        if ((u = find(u)) == (v = find(v))) return;
        if (sz[u] < sz[v]) fa[u] = v, sz[v] += sz[u], hist[++top] = u;
        else fa[v] = u, sz[u] += sz[v], hist[++top] = v;
    }
    void restore(int to) { for (; top > to; top--) sz[fa[hist[top]]] -= sz[hist[top]], fa[hist[top]] = hist[top]; }
} f;
std::vector<std::pair<int,int>> edgs[N];
int p, q, n, m;
void solve(int l, int r) {
    if (l == r) {
        int sz = f.sz[f.find(1)];
        if (sz == n) {
            ++p;
            if (m - (int)edgs[l].size() == n-1) ++q;
        }
        return;
    }
    int current = f.top;
    int mid = l + r >> 1;
    for (int i = mid+1; i <= r; i++)
        for (auto [u, v] : edgs[i])
            f.merge(u, v);
    solve(l, mid);
    f.restore(current);
    for (int i = l; i <= mid; i++)
        for (auto [u, v] : edgs[i])
            f.merge(u, v);
    solve(mid+1, r);
    f.restore(current);
}
int main() {
    int k;
    scanf("%d%d%d", &n, &m, &k);
    for (int u, v, c, i=1; i<=m; i++) {
        scanf("%d%d%d", &u, &v, &c);
        edgs[c].emplace_back(u, v);
    }
    f.init(n);
    solve(0, k-1);
    printf("%d %d\n", p, q);
    return 0;
}
