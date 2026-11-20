#include <bits/stdc++.h>
constexpr int N = 2000005;
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
        while (ch < '0') ch = gc();
        for (x = 0; ch >= '0'; ch = gc())
            x = x * 10 + (ch ^ '0');
    }
} using FastI::in;
int fa[N], deg[N], idx[N], to[N], tob[N], dep[N], q[N], bfn[N], f[N][2], premx[N], sufmx[N], sum[N], end[N];
int main() {
    freopen("dep.in", "r", stdin);
    freopen("dep.out", "w", stdout);
    int T, n; in(T);
    while (T--) {
        in(n);
        memset(deg, 0, sizeof(int) * (n+2));
        for (int i = 2; i <= n; i++)
            in(fa[i]), ++deg[fa[i]];
        for (int i = 1; i <= n; i++)
            idx[i] = deg[i-1], deg[i] += deg[i-1];
        for (int i = 2; i <= n; i++)
            to[++idx[fa[i]]] = i;
        q[1] = 1, bfn[1] = 1, dep[1] = 1; int top = 1;
        memset(end, 0, sizeof(int) * (n+2));
        for (int i = 1; i <= n; i++) {
            end[dep[q[i]]] = i;
            for (int j = deg[q[i]-1]+1; j <= deg[q[i]]; j++)
                q[bfn[to[j]]=++top] = to[j], dep[to[j]] = dep[q[i]] + 1;
        }

        memset(sum, 0, sizeof(int) * (top + 2));
        for (int d = top; d >= 1; d--) {
            int L = end[d-1]+1, R = end[d];
            for (int i = L; i <= R; i++) {
                f[i][0] = sum[d+1] + premx[end[d+1]];
                if (deg[q[i]-1] == deg[q[i]]) f[i][1] = sum[d+1] + premx[end[d+1]];
                else {
                    f[i][1] = sum[d+1];
                    if (bfn[to[deg[q[i]-1]+1]] != end[d]+1) f[i][1] = std::max(f[i][1], sum[d+1] + premx[bfn[to[deg[q[i]-1]+1]] - 1]);
                    if (bfn[to[deg[q[i]]]] != end[d+1]) f[i][1] = std::max(f[i][1], sum[d+1] + sufmx[bfn[to[deg[q[i]]]] + 1]);
                    for (int j = deg[q[i]-1]+1; j <= deg[q[i]]; j++) f[i][1] = std::max(f[i][1], sum[d+1] + f[bfn[to[j]]][1] - f[bfn[to[j]]][0] - 1);
                    f[i][1] += deg[q[i]] - deg[q[i]-1];
                }
                sum[d] = std::max(sum[d], f[i][0]);
            }
            premx[L] = std::max(f[L][1] - f[L][0], 0);
            for (int i = L+1; i <= R; i++) premx[i] = std::max(premx[i-1], f[i][1] - f[i][0]);
            sufmx[R] = std::max(f[R][1] - f[R][0], 0);
            for (int i = R-1; i >= L; i--) sufmx[i] = std::max(sufmx[i+1], f[i][1] - f[i][0]);
        }
        printf("%d\n", std::max(f[1][0] + 1, f[1][1]));
    }
}