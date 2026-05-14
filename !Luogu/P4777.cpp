#include <cstdio>
#include <cstdint>
#include <cassert>
using i128 = __int128;
constexpr int N = 100005;
i128 exgcd(i128 a, i128 b, i128& x, i128& y) {
    if (!b) { x = 1, y = 0; return a; }
    i128 g = exgcd(b, a % b, y, x);
    y -= a/b*x;
    return g;
}
i128 gcd(i128 x, i128 y) {
    i128 z;
    while (y) z = x, x = y, y = z % y;
    return x;
}
int64_t m[N], a[N];
int main() {
    int n;
    scanf("%d", &n);
    // X = m1 x1 + a1 = m2 x2 + a2
    i128 x = 0, M = 1; // Initial: x === 0 (mod 1)
    for (int i = 1; i <= n; i++) {
        int64_t m, a;
        scanf("%ld%ld", &m, &a);
        i128 g = gcd(M, m);
        assert(0 == (x - a) % g);
        i128 x1, x2;
        exgcd(m, M, x2, x1); // m2 x2 + m1 -x1 = a1 - a2
        M *= m / g;
        x = (((x - a) / g * x2 * m + a) % M + M) % M;
    }
    printf("%ld\n", (int64_t)x);
    return 0;
}