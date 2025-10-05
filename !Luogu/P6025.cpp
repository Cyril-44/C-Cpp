#include <cmath>
#include <cstdio>
inline long long calc(long long x) {
    if (x == 0) return 0;
    if (x == 1) return 1;
    // if (x == 2) return 3;
    // if (x == 3) return 7;
    // if (x == 4) return 0;
    int b = log2(x);
    x ^= 1ll << b;
    long long ret = 0;
    for (int i = b - 1; i > 1; i -= 2)
        ret |= 1ll << i;
    if (b & 1) ret |= 2;
    if (!x) return ret;
    long long tp = (1ll << b+1) | 1ll;
    ret ^= tp; --x;
    for (int i = 1; x > 0; i++)
        x -= 1ll << i, tp |= 1ll << (b+1 - i);
    if (abs(x % 2) == 1) ret ^= tp;
    return ret;
}
int main() {
    long long l, r;
    scanf("%lld%lld", &l, &r);
    printf("%lld\n", calc(r) ^ calc(l-1));
    return 0;
}