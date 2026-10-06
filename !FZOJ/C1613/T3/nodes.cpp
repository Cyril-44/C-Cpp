#include <bits/stdc++.h>
#ifdef CLANGD
constexpr int N = 25, K = 3;
#else
constexpr int N = 200005, K = 18;
#endif
int a[N], fa[N];
inline int find(int u) { return fa[u] == u ? u : (fa[u] = find(fa[u])); }
inline bool unite(int u, int v) {
    u = find(u), v = find(v);
    if (u == v) return false;
    fa[u] = v; return true;
}
struct Node {
    int mx = -1, mx2 = -1;
    Node& operator+=(int o) {
        if (o == -1) return *this;
        if (mx == -1 || a[o] > a[mx]) {
            if (mx == -1 || find(o) == find(mx)) mx = o;
            else mx2 = mx, mx = o;
        }
        else if ((mx2 == -1 || a[o] > a[mx2]) && find(o) != find(mx))
            mx2 = o;
        return *this;
    }
    Node& operator+=(const Node &o) { return (*this += o.mx) += o.mx2; }
    friend Node operator+(Node x, const Node &y) { return x += y; }
} f[1<<K];
std::vector<int> co[N]; // Component 连通块的点
std::pair<int,int> edg[N]; // 待连接的边
int main() {
    int n;
    scanf("%d", &n);
    int64_t sum = 0;
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]), sum -= a[i];
    std::iota(fa, fa+1+n, 0);
    int cnt = 0;
    while (cnt < n) {
        for (int i = 0; i <= n; i++) co[i].clear();
        for (int i = 0; i <= n; i++) co[find(i)].push_back(i);
        for (int s = 0; s < (1<<K); s++) f[s] = {};
        for (int i = 0; i <= n; i++)
            f[a[i]] += i;
        for (int k = 0; k < K; k++)
            for (int s = 0; s < (1<<K); s++)
                if (s >> k & 1) f[s] += f[s ^ (1<<k)];
        for (int i = 0; i <= n; i++) {
            if (co[i].empty()) continue;
            int mxval = -1, mxu = -1, mxv = -1;
            for (int u : co[i]) {
                auto cand = f[(1<<K)-1 ^ a[u]];
                int mx = find(cand.mx) == i ? cand.mx2 : cand.mx;
                if (mx == -1) continue;
                int val = a[u] + a[mx];
                if (val > mxval) mxval = val, mxu = u, mxv = mx;
            }
            edg[i] = {mxu, mxv};
        }
        for (int i = 0; i <= n; i++) {
            if (co[i].empty() || edg[i].first == -1) continue;
            if (unite(edg[i].first, edg[i].second))
                sum += a[edg[i].first] + a[edg[i].second], ++cnt;
        }
    }
    printf("%lld\n", sum);
    return 0;
}