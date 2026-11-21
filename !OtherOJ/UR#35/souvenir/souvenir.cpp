#include <bits/stdc++.h>
constexpr int N = 85, M = 20005;
int w[N], v[N], f[M];
int main() {
    int n, m, s, t;
    scanf("%d%d%d%d", &n, &m, &s, &t);
    for (int i = 1; i <= n; i++) scanf("%d", &w[i]);
    for (int i = 1; i <= n; i++) scanf("%d", &v[i]);
    memset(f, -0x3f, sizeof f);
    f[0] = 0;
    for (int i = 1; i <= n; i++)
        for (int j = m; j >= w[i]; j--)
            f[j] = std::max(f[j], f[j - w[i]] + v[i]);
    printf("%d\n", m >= n-1 ? f[m - (n-1)] : -1);
}