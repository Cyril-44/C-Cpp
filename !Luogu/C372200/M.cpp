#include <bits/stdc++.h>
constexpr int N = 1000005;
int a[N], w[N];
int main() {
    int n, M;
    scanf("%d%d", &n, &M);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    for (int i = 2; i <= M; i++)
        for (int j = i; j <= M; j += i)
            ++w[j];
    w[0] = M - 1;
    int64_t ans = 0;
    for (int i = 1; i <= n; i++) {
        for (int l = i-1, r = i+1, x = 0; l >= 1 && r <= n; l--, r++) {
            x = std::__gcd(x, std::abs(a[l] - a[r]));
            ans += w[x];
        }
        for (int l = i, r = i+1, x = 0; l >= 1 && r <= n; l--, r++) {
            x = std::__gcd(x, std::abs(a[l] - a[r]));
            ans += w[x];
        }
    }
    printf("%ld\n", ans);
}