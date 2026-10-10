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
constexpr int N = 10005, M = 5005;
int a[N], h[N];
int main() {
    int Q, n, m, k;
    for (in(Q); Q--; ) {
        memset(h, 0, sizeof h);
        in(n), in(m), in(k);
        int ans = 0;
        for (int i = 1; i <= n; i++)
            in(a[i]), ++h[a[i]], ans = std::max(ans, a[i]);
        std::sort(a+1, a+1+n);
        if (k == n-1) {
            int cnt = 0, sum = 0;
            for (int i = m; i >= 1; i--)
                if (h[i]) cnt += h[i], sum += i * h[i];
            if (cnt >= 2) ans = std::max(ans, (cnt - 1) * (m+1) - sum + a[1] + a[2]);
            printf("%d\n", ans);
        } else {

        }
    }
}