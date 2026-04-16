#include <cstdio>
int main() {
    freopen("year.in", "r", stdin);
    freopen("year.out", "w", stdout);
    int C, t, a, b, c;
    long long m, n;
    scanf("%d%d", &C, &t);
    for (int i = 1; i <= t; i++) {
        scanf("%lld%d%d%d%lld", &m, &a, &b, &c, &n);
        --m; n += m / a - m / b + m / c;
        c /= b, b /= a;
        long long ans = a * (n / (b * c - c + 1)) * (b * c);
        n %= b * c - c + 1;
        if (n) {
            ans += a * ((n - 1) / (b - 1)) * b;
            n = (n-1) % (b-1) + 1;
            ans += a * n;
        }
        printf("%lld\n", ans);
    }
}