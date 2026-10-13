#include <bits/stdc++.h>
namespace FastI {
    char buf[1 << 20], *p1=buf, *p2=buf;
    inline char gc() {
        if (p1 == p2) {
            p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
            if (p1 == p2) return EOF;
        }
        return *p1++;
    }
    template<class T> inline void in(T &x) {
        char ch = gc();
        while (ch < '0' || ch > '9') ch = gc();
        for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
} using FastI::in;
#ifdef CLANGD
constexpr int N = 25, M = 45;
#else
constexpr int N = 200005, M = 400005;
#endif
constexpr int64_t INF = 0x3f3f3f3f'3f3f3f3fll;
std::tuple<int,int,int,int> edgs[M];
std::vector<std::pair<int,int64_t>> g[N];
int64_t dis[N];
inline void dij(int s) {
    std::priority_queue<std::pair<int64_t, int>> pq;
    memset(dis, 0x3f, sizeof dis);
    pq.emplace(dis[s] = 0, s);
    while (!pq.empty()) {
        auto [wu, u] = pq.top(); pq.pop();
        for (auto [v, w] : g[u])
            if (dis[v] > -wu + w)
                pq.emplace(-(dis[v] = -wu + w), v);
    }
}
int main() {
    freopen("courier.in", "r", stdin);
    freopen("courier.out", "w", stdout);
    int n, m; in(n), in(m);
    bool czero = true, dag = true;
    for (int u, v, w, c, i = 1; i <= m; i++) {
        in(u), in(v), in(w), in(c);
        edgs[i] = {u, v, w, c};
        if (c) czero = false;
        if (u > v) dag = false;
    }
    if (czero) {
        for (int i = 1; i <= m; i++) {
            auto [u, v, w, c] = edgs[i];
            g[v].emplace_back(u, w);
        }
        dij(n);
        for (int i = 1; i < n; i++)
            printf("%lld ", dis[i] == INF ? -1 : dis[i]);
        puts("0");
    } else if (dag) {
        std::sort(edgs+1, edgs+1+m, [](const auto &x, const auto &y) {
            return std::get<1>(x) != std::get<1>(y) ? std::get<1>(x) < std::get<1>(y) : std::get<2>(x) > std::get<2>(y);
        });
        int prev = -1; int64_t prec = 0;
        for (int i = 1; i <= m; i++) {
            auto [u, v, w, c] = edgs[i];
            if (prev != v) prev = v, prec = 0;
            g[v].emplace_back(u, w + prec);
            prec += c;
        }
        dij(n);
        for (int i = 1; i < n; i++)
            printf("%lld ", dis[i] == INF ? -1 : dis[i]);
        puts("0");
    }
    return 0;
}