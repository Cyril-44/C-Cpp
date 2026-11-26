#include <bits/stdc++.h>
constexpr int N = 200005;
int x[N], y[N];
unsigned a[N], z[N];
int main() {
    int T, n, m, k;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d%d", &n, &m, &k);
        memset(a, 0, sizeof(int) * (n+1));
        bool nosol = false;
        for (int i = 1; i <= m; i++) {
            scanf("%d%d%u", &x[i], &y[i], &z[i]);
            a[x[i]] |= z[i], a[y[i]] |= z[i];
        }
        for (int i = 1; i <= m; i++)
            if ((a[x[i]] & a[y[i]]) != z[i])
                { nosol = true; break; }
        if (nosol) puts("-1");
        else printf("%llu\n", std::accumulate(a+1, a+1+n, 0ULL));
    }
}