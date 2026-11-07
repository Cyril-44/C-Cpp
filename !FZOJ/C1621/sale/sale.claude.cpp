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
std::vector<int> g[N];
int dsu[N], par[N], mx[N], sz[N], nxt[N], tin[N], tout[N];
int jmp[N][K+1];

inline int fnd(int x) {                       // 迭代路径压缩
    int r = x;
    while (dsu[r] != r) r = dsu[r];
    while (dsu[x] != r) { int y = dsu[x]; dsu[x] = r; x = y; }
    return r;
}

int main() {
    int Tid, n, m, q;
    in(Tid), in(n), in(m), in(q);
    for (int u, v; m--; ) {
        in(u), in(v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    // 1. 建重构树 + 求 jmp[i][0]
    for (int i = 1; i <= n; i++) dsu[i] = i, sz[i] = 1;
    for (int i = 1; i <= n; i++) {
        int best = i;
        for (int j : g[i]) if (j > best) best = j;   // i 自己及编号更大的邻居
        mx[i] = best;
        for (int j : g[i]) if (j < i) {
            int r = fnd(j);
            if (r != i) {                             // 已经并过就跳过
                dsu[r] = i, par[r] = i;
                mx[i] = std::max(mx[i], mx[r]);
            }
        }
        jmp[i][0] = mx[i];
    }
    // 2. 子树大小：par[i] > i，正序累加即可
    for (int i = 1; i <= n; i++) if (par[i]) sz[par[i]] += sz[i];
    // 3. DFS 序：par[i] > i，倒序处理保证父亲先于孩子
    for (int i = n, cur = 0; i >= 1; i--) {
        if (!par[i]) tin[i] = cur, cur += sz[i];
        else tin[i] = nxt[par[i]], nxt[par[i]] += sz[i];
        nxt[i] = tin[i] + 1;
        tout[i] = tin[i] + sz[i];
    }
    // 4. 倍增表
    for (int k = 1; k <= K; k++)
        for (int i = 1; i <= n; i++)
            jmp[i][k] = jmp[jmp[i][k-1]][k-1];
    // 5. 询问
    for (int s, t; q--; ) {
        in(s), in(t);
        int tt = tin[t];
        auto inSub = [&](int p) { return tin[p] <= tt && tt < tout[p]; };
        if (inSub(s)) { out(1); continue; }
        int cnt = 2;
        for (int k = K; k >= 0; k--) {
            int p = jmp[s][k];
            if (!inSub(p)) s = p, cnt += 1 << k;
        }
        out(cnt);
    }
    return 0;
}