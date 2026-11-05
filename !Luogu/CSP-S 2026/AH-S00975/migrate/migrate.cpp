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
constexpr int N = 2000005;
LL x[N], F1a[N], F1b[N], F2[N];
int nxt[N], pre[N];
int main() {
    freopen("migrate.in", "r", stdin);
    freopen("migrate.out", "w", stdout);
    int Tid, T, n; LL d;
    in(Tid), in(T);
    while (T--) {
        in(n), in(d);
        for (int i = 1; i <= n; i++) in(x[i]);
        for (int i = 1, r = 1; i <= n; i++) {
            if (r < i) r = i;
            while (r < n && x[r+1] - x[i] <= d) r++;
            nxt[i] = r;
        }
        for (int i = 1, l = 1; i <= n; i++) {
            while (x[i] - x[l] > d) l++;
            pre[i] = l;
        }
        nxt[0] = 0;
        for (int i = n; i >= 1; i--) {
            int j = nxt[i];
            if (j == i) { F1a[i] = F1b[i] = F2[i] = 0; continue; }
            int o = i - pre[j];                                   // 你的 opts[i]
            LL cR = std::max(0, j - std::max(i, nxt[i-1]));       // 状态2：下一层 pre[w]==i 的点数
            LL cA = i >= 2 ? std::max(0, nxt[i-1] - std::max(i, nxt[i-2])) : 0; // 状态1a：pre[w]==i-1 且 w>i
            F2[i] = cR + (o == 0 ? F2[j] : 0);
            LL go = 0;                                            // o==0：沿链继续，α 保持 1
            if (o == 0) go = (j >= i + 2 && pre[j-1] == i) ? F1a[j] : F1b[j];
            F1a[i] = cA + (o == 0 ? go : o == 1 ? F2[j] : 0);     // o==1：1+1=2，进入状态2
            F1b[i] = go;                                          // 1b 下 o>=1 时 α+β>=3，停止
        }
        LL ans = 0;
        for (int u = 1; u <= n; u++)
            if (nxt[u] != u) ans += (nxt[u] >= u + 2) ? F1a[nxt[u]] : F1b[nxt[u]];
        printf("%lld\n", ans);
    }
}