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
        while (ch < '0') ch = gc();
        for (x = 0; ch >= '0'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
} using FastI::in;
constexpr int N = (int)1e6 + 5;
#ifdef CLANGD
#define N 15
#endif
int a[N], h[N], hh[N];
constexpr uint64_t B = 10007;
uint64_t base[N];
int main() {
    int T, n, m;
    in(T);
    while (T--) {
        in(n), in(m);
        for (int i = 1; i <= n; i++) in(a[i]);
        memset(h, 0, sizeof h);
        memset(hh, 0, sizeof hh);
        base[0] = 1;
        for (int i = 1; i <= n; i++) base[i] = base[i-1] * B;
        uint64_t allone = 0;
        for (int i = 1; i <= m; i++) allone += base[i];
        int mn = 0;
        hh[0] = m;
        uint64_t curr = 0;
        std::unordered_map<uint64_t, int> mp;
        mp[0] = 1;
        uint64_t ans = 0;
        for (int i = 1; i <= n; i++) {
            if (--hh[h[a[i]]] == 0 && mn == h[a[i]]) ++mn, curr -= allone;
            ++hh[++h[a[i]]], curr += base[a[i]];
            ans += mp[curr]++;
        }
        printf("%llu\n", ans);
    }
}