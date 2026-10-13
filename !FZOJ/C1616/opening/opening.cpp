#include <bits/stdc++.h>
constexpr int N = 200005;
int r[N], c[N];
int64_t premn[N], sufmn[N], sum[N];
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d%d", &c[i], &r[i]);
    for (int i = 1; i <= n; i++)
        sum[i] = sum[i-1] - c[i] + r[i];
    premn[0] = sufmn[n+1] = 1ll << 60;
    for (int i = 1; i <= n; i++)
        premn[i] = std::min(premn[i-1], sum[i-1] - c[i]);
    for (int i = n; i >= 1; i--)
        sufmn[i] = std::min(sufmn[i+1] - c[i] + r[i], (int64_t)-c[i]);
    int64_t ans = 1ll << 60; int cnt = 0;
    for (int i = 1; i <= n; i++) {
        int64_t mn = -std::min({(int64_t)-c[i], -c[i] + r[i] + premn[i-1], sum[i] + sufmn[i+1]});
        if (mn < ans) ans = mn, cnt = 1;
        else if (mn == ans) ++cnt;
    }
    printf("%lld %d\n", ans, cnt);
}