#include <bits/stdc++.h>
namespace FastI {
    char buf[1 << 20], *p1, *p2;
    inline char gc() {
        if (p1 == p2) {
            p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
            if (p1 == p2) return EOF;
        }
        return *p1++;
    }
    template<typename T> inline void in(T &x) {
        char ch = gc();
        while (ch < '-') ch = gc();
        bool neg = false;
        if (ch == '-') neg = true, ch = gc();
        for (x = 0; ch >= '0'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
        if (neg) x = -x;
    }
} using FastI::in;
using LL = long long;
constexpr int N = 2000005, K = 20;
LL x[N];
int nxt[N], reach[N], length[N], pre[N], opts[N], cnt[N], val[N];
LL vals[N];
#define fprintf(...) (void)0
int main() {
    freopen("migrate.in", "r", stdin);
    freopen("migrate.out", "w", stdout);
    int Tid, T, n; LL d;
    in(Tid), in(T);
    while (T--) {
        in(n), in(d);
        for (int i = 1; i <= n; i++) in(x[i]);
        for (int i = 1; i <= n; i++) {
            nxt[i] = std::upper_bound(x+1, x+1+n, x[i] + d) - x - 1;
            pre[i] = std::lower_bound(x+1, x+1+n, x[i] - d) - x;
            ++cnt[pre[i]];
        }
        for (int i = 1; i <= n; i++) {
            --cnt[pre[i]];
            vals[i] = val[i] = cnt[i-1];
        }
        for (int i = 1; i <= n; i++) fprintf(stderr, "%d%c", val[i], " \n"[i==n]);
        for (int i = n; i >= 1; i--) {
            opts[i] = i - pre[nxt[i]];
            if (opts[i] || i == nxt[i]) reach[i] = i, length[i] = 0;
            else {
                if (nxt[i] == i+1 || pre[nxt[i]-1] == i) vals[i] += vals[nxt[i]];
                reach[i] = reach[nxt[i]], length[i] = length[nxt[i]] + 1;
            }
        }
        for (int i = 1; i <= n; i++) fprintf(stderr, "%d%c", nxt[i], " \n"[i==n]);
        for (int i = 1; i <= n; i++) fprintf(stderr, "%d%c", pre[i], " \n"[i==n]);
        for (int i = 1; i <= n; i++) fprintf(stderr, "%d%c", opts[i], " \n"[i==n]);
        for (int i = 1; i <= n; i++) fprintf(stderr, "%d%c", cnt[i], " \n"[i==n]);
        for (int i = 1; i <= n; i++) fprintf(stderr, "%d%c", reach[i], " \n"[i==n]);
        for (int i = 1; i <= n; i++) fprintf(stderr, "%d%c", length[i], " \n"[i==n]);
        for (int i = 1; i <= n; i++) fprintf(stderr, "%d%c", vals[i], " \n"[i==n]);
        LL ans = 0;
        for (int i = 1; i <= n; i++) {
            int p1 = reach[nxt[i]];
            ans += vals[nxt[i]];
            fprintf(stderr, "i=%d, p1=%d, {A} ans += vals[nxt[i]] (%lld)\n", i, p1, vals[nxt[i]]);
            if (opts[p1] == 1) {
                int p2 = nxt[p1];
                ans += length[p2];
                fprintf(stderr, "i=%d, {B} ans += length[%d] (%d)\n", i, p2, length[p2]);
            }
        }
        printf("%lld\n", ans);
    }
}