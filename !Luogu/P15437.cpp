#include <stdio.h>
#include <algorithm>
struct Frac {
    int x, y;
    inline Frac(int a, int b) {
        int c = std::__gcd(a, b);
        a /= c, b /= c;
        x = a, y = b;
    }
    inline bool operator<(const Frac &o) const {
        return 1ull * x * o.y < 1ull * y * o.x;
    }
};
int main() {
    int n, x, y;
    scanf("%d%d%d", &n, &x, &y);
    Frac mn(x, y), mx(x, y);
    for (int i = 1; i < n; i++) {
        scanf("%d%d", &x, &y);
        mn = std::min(mn, Frac(x, y));
        mx = std::max(mx, Frac(x, y));
    }
    long long d = 1ull * mn.x * mx.y, e = 1ull * mx.x * mn.y;
    long long f = std::__gcd(d, e);
    d /= f, e /= f;
    printf("%lld %lld\n", d, e);
    return 0;
}