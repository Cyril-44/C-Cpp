#include <bits/stdc++.h>
constexpr int N = 500005;
using LL = long long;
LL a[N], s[N];
int main() {
    freopen("crystal.in", "r", stdin);
    freopen("crystal.out", "w", stdout);
    int n;
    LL m;
    scanf("%d%lld", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%lld", &a[i]);
    std::sort(a+1, a+1+n);
    for (int i = 1; i <= n; i++)
        s[i] = s[i-1] + a[i];
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        int l = 1, r = i;
        while (l <= r) {
            int k = l + r >> 1;
            if (a[i] * k - (s[i] - s[i-k]) <= m) l = k + 1;
            else r = k - 1;
        }
        // fprintf(stderr, "%d: %d\n", i, r);
        ans = std::max(ans, r);
    }
    printf("%d\n", ans);
}