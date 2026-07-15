#include <bits/stdc++.h>
struct FastI {
    char buf[1 << 20], *p1, *p2;
    FILE* f;
    FastI(const char* fn) : p1(nullptr), p2(nullptr), f(fopen(fn, "r")) {}
    ~FastI() { fclose(f); }
    char get() {
        if (__builtin_expect(p1 == p2, 0)) p2 = (p1=buf) + fread(buf, 1, sizeof buf, f);
        return *p1++;
    }
    void operator()(int &x) {
        char ch = get();
        while (ch < '0' || ch > '9') ch = get();
        for (x = 0; ch >= '0' && ch <= '9'; ch = get())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
};
struct FastO {
    char buf[1 << 20], *p3;
    const char* p4;
    FILE* f;
    FastO(const char* fn) : p3(buf), p4(buf + sizeof buf), f(fopen(fn, "w")) {}
    void flush() { fwrite(buf, p3-buf, 1, f); }
    ~FastO() { flush(); fclose(f); }
    void put(char ch) {
        if (__builtin_expect(p3 == p4, 0)) fwrite(buf, sizeof buf, 1, f), p3 = buf;
        *p3++ = ch;
    }
    int8_t sta[20];
    void operator()(int64_t x) {
        sta[sta[0] = 1] = x % 10;
        while (x /= 10) sta[++sta[0]] = x % 10;
        while (sta[0]) put(sta[sta[0]--] | '0');
    }
};

#define For(i, s, t) for (int i = (s); i <= (t); i++)
#define Rep(i, n) for (int i = 0; i < n; i++)
constexpr int N = 500005;
int n;
std::vector<int> g[N];
int sz[N], dep[N];

namespace BF {
    int chain[N];
    int64_t sum[N];
    void dfs1(int u, int fa) {
        sz[u] = 1, sum[u] = dep[u];
        for (int v : g[u]) if (v != fa) {
            dep[v] = dep[u] + 1;
            dfs1(v, u);
            sz[u] += sz[v], sum[u] += sum[v];
        }
    }
    inline int64_t sumof(int dep) { return sum[chain[dep]] - sum[chain[dep+1]] - 1ll * dep * (sz[chain[dep]] - sz[chain[dep+1]]); }
    inline int64_t getsize(int dep) { return sz[chain[dep]] - sz[chain[dep+1]]; }
    inline int64_t sumofp(int u) { return sum[u] - 1ll * dep[u] * sz[u]; }
    int64_t sumrt, ans;
    void dfs2(int u, int fa, int64_t w1, int64_t w2) { // w1 是当前链中点往下所有点的 sz[u]-sz[v] 和，w2 是当前上方的和
        chain[dep[u]] = u;
        if (dep[u] > 2) {
            if (dep[u] & 1) { // 链上有偶数个点
                w1 += getsize(dep[u]-1);
                w2 += w1 + sumof(dep[u]-1);
            }
            else { // 链上有奇数个点（下方移除了 chain[dep[u]/2]）
                w1 = w1 - getsize(dep[u]/2) + getsize(dep[u]-1);
                w2 = w2 - sumof(dep[u]/2) - getsize(dep[u]/2) * (dep[u]/2 - 1) + w1 + sumof(dep[u]-1);
            }
        }
        if (fa) {
            int mu = chain[dep[u]/2 + 1];
            // fprintf(stderr, "%d: d=%d, %ld %ld, ans<-%ld-%ld+%ld+%ld-%ld=%ld (mu=%d)\n",
            //         u, dep[u], w1, w2, sumrt, 1ll*dep[u] * sz[u], w2, sumofp(u), sumofp(mu), sumrt - (int64_t)dep[u] * sz[u] + w2 + sumofp(u) - sumofp(mu), mu);
            ans = std::min(ans, sumrt - sum[mu] + w2 + sumofp(u));
        }
        for (int v : g[u]) if (v != fa)
            dfs2(v, u, w1, w2);
    }
    inline int64_t calc(int a) {
        // fprintf(stderr, "Calculating a=%d...\n", a);
        dep[a] = 0; 
        dfs1(a, 0);
        sumrt = sum[a], ans = sumrt;
        dfs2(a, 0, 0, 0);
        return ans;
    }
    inline int64_t work1() {
        int64_t res = 1ll << 60;
        for (int i = 1; i <= n; i++)
            res = std::min(res, calc(i));
        return res;
    }
    inline int64_t work2() {
        return calc(1);
    }
    inline std::vector<int64_t> work3() {
        std::vector<int64_t> vans(n);
        for (int i = 1; i <= n; i++)
            vans[i-1] = calc(i);
        return vans;
    }
}

int main() {
    FastI in("house.in"); FastO out("house.out");
    int T, o; in(T);
    Rep(t_, T) {
        in(n), in(o);
        For(i, 1, n) g[i].clear();
        Rep(i_, n-1) {
            int u, v; in(u), in(v);
            g[u].push_back(v);
            g[v].push_back(u);
        }
        switch (o) {
        case 1: if (n <= 4000) out(BF::work1()); else return 0; break;
        case 2: out(BF::work2()); break;
        case 3: if (n <= 4000) for (auto v : BF::work3()) out(v), out.put(' '); else return 0;
        }
        out.put('\n');
    }
    return 0;
}