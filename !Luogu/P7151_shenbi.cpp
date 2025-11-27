#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int O = 307, N = 607, M = 180007, I = 1e6;
int c[N];
ll d[N], a[O][O], e[N][N];
bool f[N];
#define _                                                                      \
    {                                                                          \
        if (d[j] > (l = d[i] + e[i][j])) {                                     \
            if (d[j] = l, (c[j] = c[i] + 1) == u) {                            \
                puts("NO");                                                    \
                goto gg;                                                       \
            }                                                                  \
            if (!f[j])                                                         \
                f[j] = 1, q.push(j);                                           \
        }                                                                      \
    }
int main() {
    int T, n, m, i, j, o, u;
    ll l;
    for (scanf("%d", &T); T--;) {
        scanf("%d%d", &n, &m), memset(a, 0, sizeof a),
            u = min(n + m, 99); // u用来卡松弛次数
        for (i = 2; i <= n; ++i)
            for (j = 2; j <= m; ++j)
                scanf("%d", &o), a[i][j] = o - a[i - 1][j - 1] - a[i - 1][j] -
                                           a[i][j - 1]; //递推求A
        for (i = 1; i <= n; ++i)
            for (j = 1; j <= m; ++j)
                if (i + j & 1)
                    e[j + n][i] = a[i][j], e[i][j + n] = I - a[i][j];
                else
                    e[j + n][i] = I - a[i][j], e[i][j + n] = a[i][j]; //连边
        queue<int> q;
        memset(d, 9, sizeof d), memset(f, 0, sizeof f), d[1] = c[1] = 0,
                                                        q.push(1), o = n + m;
        while (q.size())
            if (f[i = q.front()] = 0, q.pop(), i <= n)
                for (j = n + 1; j <= o; ++j)
                    _ else for (j = 1; j <= n; ++j) _ // spfa
                        for (puts("YES"), i = 1; i <= n;
                             ++i, puts("")) for (j = 1; j <= m; ++j)
                            printf("%lld ",
                                   a[i][j] + (d[j + n] - d[i]) *
                                                 ((i + j & 1) ? 1 : -1));
    gg:;
    }
    return 0;
}
