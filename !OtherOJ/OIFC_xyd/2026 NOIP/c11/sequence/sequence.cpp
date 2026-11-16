#include <bits/stdc++.h>
constexpr int N = 1000005;
int a[N], vis[N];
int64_t f[N];
int main() {
    freopen("sequence.in", "r", stdin);
    freopen("sequence.out", "w", stdout);
    int T, n, m;
    scanf("%*d%d", &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
        std::fill(f+1, f+1+n, 1);
        memset(vis, 0, sizeof(int) * (m+1));
        int64_t ans = 0;
        for (int i = 1, j = 0, cnt = 0; i <= n; i++) {
            while (cnt < m && j < n)
                if (!vis[a[++j]]++) ++cnt;
            if (cnt < m) break;
            if (!--vis[a[i]]) --cnt;
            ans += f[i] * (n - j + 1);
            f[j+1] += f[i];
        }
        printf("%lld\n", ans);
    }
    return 0;
}