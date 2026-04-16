#include <cstdio>
int main() {
    freopen("candy.in", "r", stdin);
    freopen("candy.out", "w", stdout);
    long long n, x, m, k;
    scanf("%lld%lld%lld%lld", &n, &x, &m, &k);
    if (n < m * k)
        printf("-1");
    else
        printf("%lld\n", (n - m * k) * x);
    return 0;
}