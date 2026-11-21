#include <bits/stdc++.h>
constexpr int N = 25;
int p[N], a[N], f[N];
bool vis[N];
int main() {
    int T, n;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        memset(vis, 0, sizeof vis);
        for (int i = 1; i <= n; i++) {
            scanf("%d", &p[i]);
            if (p[i] != -1) vis[p[i]] = true;
        }
        int m = 0, ans = 0;
        for (int i = 1; i <= n; i++)
            if (!vis[i]) a[++m] = i;
        do {
            for (int i = 1, j = 1; i <= n; i++)
                if (p[i] == -1) f[i] = a[j++];
                else f[i] = p[i];
            int res = std::abs(f[1] - f[n]);
            for (int i = 2; i <= n; i++) res += std::abs(f[i] - f[i-1]);
            ans = std::max(ans, res);
        } while (std::next_permutation(a+1, a+1+m));
        printf("%d\n", ans);
    }
}