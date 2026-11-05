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
    inline void in(int &x) {
        char ch = gc();
        while (ch < '0') ch = gc();
        for (x = 0; ch >= '0'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
} using FastI::in;
constexpr int N = 505, M = 500005;
using LL = long long;
int w[M];
std::vector<std::pair<int,int>> vl[M];
LL f[N][N];
template<typename T> inline void umin(T &x, T y) { if (x > y) x = y; }
int main() {
    freopen("subway.in", "r", stdin);
    freopen("subway.out", "w", stdout);
    int Tid, n, m, Dlmt, q;
    in(Tid), in(n), in(m), in(Dlmt), in(q);
    for (int i = 1; i <= m; i++) in(w[i]);
    for (int i = 1; i <= n; i++) {
        int k; in(k);
        for (int j = 1; j <= k; j++) {
            int a, t; in(a), in(t);
            vl[a].emplace_back(i, t);
        }
    }
    memset(f, 0x3f, sizeof f);
    for (int i = 1; i <= n; i++)
        f[i][i] = 0;
    for (int j = 1; j <= m; j++)
        for (int p1 = 0; p1 < (int)vl[j].size(); p1++)
            for (int p2 = p1+1; p2 < (int)vl[j].size(); p2++) {
                int i1 = vl[j][p1].first, i2 = vl[j][p2].first,
                    t1 = vl[j][p1].second, t2 = vl[j][p2].second;
                umin(f[i1][i2], (LL)t1 + t2);
                umin(f[i2][i1], (LL)t1 + t2);
            }
    for (int k = 1; k <= n; k++)
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                if (k != i && k != j && i != j)
                    umin(f[i][j], f[i][k] + f[k][j]);
    while (q--) {
        int x, y; in(x), in(y);
        LL ans = 0x3f3f3f3f3f3f3f3fll;
        for (auto pix : vl[x]) for (auto piy : vl[y]) {
            int ix = pix.first, tx = pix.second,
                iy = piy.first, ty = piy.second;
            // fprintf(stderr, "[%d] ==(%d)==> {%d} ==(%d)==> {%d} ==(%d)==> [%d]\n", x, tx, ix, f[ix][iy], iy, ty, y);
            umin(ans, tx + ty + f[ix][iy]);
        }
        printf("%lld\n", ans + w[x] + w[y]);
    }
}