#include <cassert>
#include <cstdio>
int main() {
    int C, t, m, a, b, c, n;
    scanf("%d%d", &C, &t);
    for (int i = 1; i <= t; i++) {
        scanf("%d%d%d%d%d", &m, &a, &b, &c, &n);
        --m; n += m / a - m / b + m / c;
        c /= b, b /= a;
        long long ans = 1ll * a * (n / (b * c - c + 1)) * (b * c); n %= b * c - c + 1;
        if (n) {
            ans += 1ll * a * ((n - 1) / (b - 1)) * b;
            n = (n-1) % (b-1) + 1;
            ans += 1ll * a * n;
        }
        printf("%lld\n", ans);
    }
}