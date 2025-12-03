#include <stdio.h>
#ifdef DEBUG
# define debug(...) fprintf(stderr, __VA_ARGS__)
#else
# define debug(...) ((void)0)
#endif
using LL = long long;
inline void swap(int &x, int &y) {
    (*(unsigned*)(&x)) ^= (*(unsigned*)(&y)) ^= (*(unsigned*)(&x)) ^= (*(unsigned*)(&y));
}
inline void swap(LL &x, LL &y) {
    using u64 = unsigned long long;
    (*(u64*)(&x)) ^= (*(u64*)(&y)) ^= (*(u64*)(&x)) ^= (*(u64*)(&y));
}
int exgcd(int a, int b, LL &x, LL &y) { // ax + by = gcd(a, b)
    if (!b) {
        x = 1, y = 0;
        return a;
    }
    int sol = exgcd(b, a % b, x, y);
    LL t = x;
    x = y;
    y = t - (a / b) * y;
    return sol;
}
int main() {
    int T;
    scanf("%d", &T);
    int a, b, c;
    while (T--) {
        scanf("%d%d%d", &a, &b, &c);
        LL x, y;
        int com = exgcd(a, b, x, y);
        debug("!!%d %d %d %lld %lld\n", a, b, c, x, y);
        if (c % com) puts("-1");
        else {
            bool swapped = false;
            LL fix = c / com;
            a /= com, b /= com; // 本质上是求解 ax + by = 1 (a 与 b 互质)
            x *= fix, y *= fix;
            if (x < y) swap(a, b), swap(x, y), swapped = true;
            if (x <= 0) goto NoPositiveSolution;
            {
                LL xMxSub = (x - 1) / b;
                x -= b * xMxSub, y += a * xMxSub;
                if (y <= 0) goto NoPositiveSolution;
                // 此时 x 已经处于最小状态
                LL yMxSub = (y - 1) / a;
                debug(">>> %lld %lld %lld %lld\n", x, y, xMxSub, yMxSub);
                if (swapped)
                    printf("%lld %lld %lld %lld %lld\n", yMxSub + 1,
                        y - yMxSub * a, x, y, x + yMxSub * b
                    );
                else 
                    printf("%lld %lld %lld %lld %lld\n", yMxSub + 1,
                        x, y - yMxSub * a, x + yMxSub * b, y
                    );
                
                goto end;
            }
            NoPositiveSolution: {
                LL x1 = (x % b + b) % b;
                if (!x1) x1 = b;
                LL x2 = (y % a + a) % a;
                if (!x2) x2 = a;
                if (swapped)
                    printf("%lld %lld\n", x2, x1);
                else
                    printf("%lld %lld\n", x1, x2);
            }
            end:;
        }
    }
    return 0;
}