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
constexpr int N = 10005;
int a[N], h[N], _f[N<<1], *f = _f+N, _g[N<<1], *g = _g+N;
int main() {
    int Q, n, m, k;
    for (in(Q); Q--; ) {
        memset(h, 0, sizeof h);
        in(n), in(m), in(k); k = n - k;
        int ans = 0;
        for (int i = 1; i <= n; i++) {
            in(a[i]);
            ans = std::max(ans, a[i]);
            if (a[i] > m) --n, --i;
            else a[i] = m + 1 - a[i];
        }
        std::sort(a+1, a+1+n);
        if (k == 1) {
            int cur = m + 1;
            for (int i = 1; i <= n-2; i++)
                cur += a[i];
            ans = std::max(ans, cur);
        } else if (n <= 4) {
            if (n > 2) ans = std::max(ans, m + 1);
        } else {
            n -= 4;
            const int sum = std::accumulate(a+1, a+1+n, 0), t = sum / 2;
            int p = 0, P = 0;
            while (p < n && P + a[p+1] <= t) P += a[++p];
            memset(_f, 0, sizeof _f);
            f[P - t] = p + 1;
            for (int i = p + 1; i <= n; i++) {
                memcpy(_g, _f, sizeof _g);                       // 不加入 b_i
                for (int j = -m; j <= 0; j++)                    // (a) 加入
                    if (f[j] && j + a[i] <= m)
                        g[j + a[i]] = std::max(g[j + a[i]], f[j]);
                for (int j = m; j >= 1; j--)                     // (b) 删除，j 从大到小
                    for (int h = g[j]-1; h >= std::max(f[j], 1); h--)
                        g[j - a[h]] = std::max(g[j - a[h]], h);
                memcpy(_f, _g, sizeof _g);
            }
            int j = 0;
            while (!f[j]) j--;
            ans = std::max(ans, m + 1 + sum - (t + j));
        }
        printf("%d\n", ans);
    }
}