#include <cstdio>
#include <cstdint>
#include <tuple>
constexpr int N = 15;
int m[N], a[N];
using i128 = __int128;
i128 exgcd(i128 a, i128 b, i128& x, i128& y) {
    if (b == 0) { x = 1, y = 0; return a; }
    auto g = exgcd(b, a%b, x, y);
    std::tie(x, y) = std::make_tuple(y, x - a/b*y);
    return g;
}
/*
x \equiv 
*/
int main() {
    int n;
    scanf("%d", &n);
    int64_t M = 1;
    i128 ans = 0;
    for (int i = 1; i <= n;  i++) {
        scanf("%d%d", &m[i], &a[i]);
        M *= m[i];
    }
    for (int i = 1; i <= n; i++) {
        i128 x, _;
        exgcd(M / m[i], m[i], x, _);
        ans += M / m[i] * (((x % m[i]) + m[i]) % m[i]) * a[i];
    }
    int64_t fin = ans % M;
    printf("%ld\n", fin);
    return 0;
}
