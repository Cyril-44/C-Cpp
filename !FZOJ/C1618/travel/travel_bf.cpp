#include <bits/stdc++.h>
namespace FastI {
    char buf[1 << 20], *p1{}, *p2{};
    inline char gc() {
        if (p1 == p2) p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
        if (p1 == p2) return EOF;
        return *p1++;
    }
    inline void in(int &x) {
        char ch = gc();
        while (ch < '0' || ch > '9') ch = gc();
        for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
} using FastI::in;
constexpr int N = 505, Q = 10005;
int a[N], b[N], s[Q], t[Q];
struct Edge { int u, v, w; };
std::vector<Edge> e[N];
std::vector<std::pair<int,int>> g[N];
int64_t f[N][N], ans[Q];
inline void dij(int s) {
    memset(f[s], 0x3f, sizeof(int64_t) * N);
    f[s][s] = 0;
    std::priority_queue<std::pair<int64_t, int>> pq;
    pq.push({0, s});
    while (!pq.empty()) {
        auto [wu, u] = pq.top(); pq.pop();
        if (f[s][u] != (wu = -wu)) continue;
        for (auto [v, w] : g[u])
            if (f[s][v] > wu + w)
                pq.emplace(-(f[s][v] = wu + w), v);
    }
}
int main() {
    int n, m, q;
    in(n), in(m);
    for (int i = 1; i <= n; i++) in(a[i]);
    for (int i = 1, u, v, w; i <= m; i++) {
        in(u), in(v), in(w);
        e[a[u] > a[v] ? u : v].push_back({u, v, w});
    }
    in(q);
    for (int i = 1; i <= q; i++) in(s[i]), in(t[i]);
    std::iota(b+1, b+1+n, 1);
    std::sort(b+1, b+1+n, [](int x, int y){ return a[x] < a[y]; });
    memset(ans, 0x3f, sizeof ans);
    const int64_t INF = ans[0];
    for (int i = 1; i <= n; i++) {
        for (const auto& [u, v, w] : e[b[i]])
            g[u].emplace_back(v, w);
        for (int j = 1; j <= n; j++) dij(j);
        for (int j = 1; j <= q; j++)
            if (f[s[j]][t[j]] != INF)
                ans[j] = std::min(ans[j], f[s[j]][t[j]] * a[b[i]]);
    }
    for (int i = 1; i <= q; i++)
        printf("%lld\n", ans[i] == INF ? -1 : ans[i]);
}