#include <stdio.h>
#include <string.h>
const int N = 100005, M = 17 /*1e5*/;
long long tr[1 << M + 1];
int _a[N], a[N], c[N], _1_k;
long long f[N];
bool op[1 << M];
inline void scanb(bool *s) {
    char ch = getchar();
    while ((ch ^ '0') && (ch ^ '1'))
        ch = getchar();
    while (ch == '0' || ch == '1')
        *(s++) = ch & 1, ch = getchar();
}
inline void flag(int u) {
    if (tr[u] ^ -1) return;
    if (u >= _1_k) tr[u] = u - _1_k + 1;
    else {
        flag(u << 1);
        flag(u << 1 | 1);
        tr[u] = tr[u << 1] + tr[u << 1 | 1];
    }
}
int main() {
#ifndef ONLINE_JUDGE
    freopen("arena/arena.in", "r", stdin);
    freopen("arena/arena.out", "w", stdout);
#endif
    int t, n, m, k, X[4];
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", &_a[i]);
    for (int i = 1; i <= m; i++)
        scanf("%d", &c[i]);
    for (k = 0; (1 << k) < n; ++k);
    _1_k = 1 << k;
    for (int i = k-1; i >= 0; i--)
        scanb(op + (1 << i));
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d%d%d", X, X+1, X+2, X+3);
        memset(tr, -1, sizeof(long long) * (_1_k<<1));
        for (int i = 1; i <= n; i++)
            a[i] = _a[i] ^ X[i & 3];
        long long sum = 1, del = 0;
        int u;
        for (int i = 1, r = 0, s2 = 1; i <= n; i++) {
            if (tr[u = i + _1_k - 1] == -1) {
                tr[u] = i;
                for (int j = 1; j <= r; j++, u >>= 1) { // 当前局数
                    if ((u & 1) && !op[u>>1] && a[tr[u^1]] < j) { // 继承
                        tr[u >> 1] = tr[u];
                    }
                    else if ((u & 1) ^ op[u>>1]) // 匹配01, 10
                        break; // 事不关己
                    else if (a[tr[u]] >= j) { // 当前赢了
                        tr[u >> 1] = tr[u];
                        flag(u ^ 1);
                        del += tr[u ^ 1];
                    }
                    else {
                        flag(u);
                        del += tr[u];
                        if (tr[u ^ 1] ^ -1)
                            tr[u >> 1] = tr[u ^ 1];
                        else break; // 等待继承
                    }
                }
            }
            f[i] = sum - del;
            if (i == s2) {  // 是 2 的方幂
                u = 1 << k - r; // 往上推一层
                ++r, s2 <<= 1;
                sum += (i+1ll + s2) * 1ll * (s2 - i) >> 1;
                if (!op[u>>1]) // 匹配00 向上推
                    if (a[tr[u]] >= r) { // 当前赢了
                        tr[u >> 1] = tr[u];
                        flag(u ^ 1);
                        del += tr[u ^ 1];
                    }
                    else { // 输了
                        flag(u);
                        del += tr[u];
                    }
            }
        }
        long long ans = 0;
        for (int i = 1; i <= m; i++)
            ans ^= f[c[i]] * i;
        printf("%lld\n", ans);
    }
    return 0;
}