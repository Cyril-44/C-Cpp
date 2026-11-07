#include <bits/stdc++.h>
namespace FastI {
    char buf[1<<20], *p1, *p2;
    inline char gc() {
        if (p1 == p2) {
            p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
            if (p1 == p2) return EOF;
        }
        return *p1++;
    }
    template<class T> inline void in(T &x) {
        char ch = gc();
        while (ch < '0') ch = gc();
        for (x = 0; ch >= '0'; ch = gc())
            x = x * 10 + (ch ^ '0');
    }
} using FastI::in;
struct FastO {
    char buf[1<<20], *p3=buf;
    const char *p4=buf+sizeof(buf);
    inline void flush() { fwrite(buf, p3 - buf, 1, stdout), p3=buf; }
    inline void pc(char c) {
        if (p3 == p4) fwrite(buf, sizeof buf, 1, stdout), p3=buf;
        *p3++ = c;
    }
    template<class T> inline void operator()(T x, char end = '\n') {
        int8_t sta[40];
        sta[sta[0] = 1] = x % 10;
        while (x /= 10) sta[++sta[0]] = x % 10;
        while (sta[0]) pc(sta[sta[0]--] | '0');
        if (end) pc(end);
    }
    ~FastO() { flush(); }
} out;
constexpr int N = 500005, K = 18;
#ifdef CLANGD
#define N 55
#endif
std::vector<int> g[N], tr[N];
std::vector<std::pair<int,int>> fa[N];
int sz[N], mx[N];
int jmp[N][K+1];
inline int find(int u) { while (u != fa[u].back().second) u = fa[u].back().second; return u; }
inline void umax(int &x, int y) { if (x < y) x = y; }
inline int find(int u, int t) {
    int f = u;
    do u = f, f = std::prev(std::upper_bound(fa[u].begin(), fa[u].end(), std::make_pair(t, (int)1e9)))->second;
    while (u != f);
    return u;
}
int main() {
    int Tid, n, m, q;
    in(Tid), in(n), in(m), in(q);
    for (int u, v; m--; ) {
        in(u), in(v);
        g[v].push_back(u);
        g[u].push_back(v);
    }
    for (int i = 1; i <= n; i++) {
        fa[i].emplace_back(0, i);
        sz[i] = 1, mx[i] = i;
    }
    for (int i = 1; i <= n; i++) {
        for (int j : g[i]) {
            if (j < i) {
                int u = find(i), v = find(j);
                if (u == v) continue;
                if (sz[u] < sz[v]) std::swap(u, v);
                mx[u] = std::max(mx[u], mx[v]);
                sz[u] += sz[v];
                fa[v].emplace_back(i, u);
            }
            else umax(mx[find(i)], j);
        }
        jmp[i][0] = mx[find(i)];
    }
    for (int k = 1; k <= K; k++)
        for (int i = 1; i <= n; i++)
            jmp[i][k] = jmp[jmp[i][k-1]][k-1];
    for (int s, t; q--; ) {
        in(s), in(t);
        if (find(s, s) == find(t, s)) {
            out(1);
            continue;
        }
        int cnt = 2;
        for (int k = K; k >= 0; k--)
            if (find(jmp[s][k], jmp[s][k]) != find(t, jmp[s][k]))
                s = jmp[s][k], cnt += 1<<k;
        out(cnt);
    }
    return 0;
}