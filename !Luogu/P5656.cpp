#include <stdio.h>
using LL = long long;
int exgcd(int u, int v, LL &x, LL &y) { // ux + vy = gcd(u, v)
    if (!v) {
        x = 1, y = 0;
        return u;
    }
    int sol = exgcd(v, u % v, x, y);
    int t = x;
    x = y;
    y = t - (u / v) * y;
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
        if (c % com) puts("-1");
        else {
            LL fix = c / com;
            x *= fix, y *= fix;
            if (x > 0 && y > 0) {
                LL xredmx = x / b, yredmx = y / a;
                printf("%lld %lld %lld %lld %lld\n", xredmx + yredmx + 1, 
                    (x - xredmx * b), (y - yredmx * a),
                    (x + yredmx * b), (y + xredmx * a)
                );
            } else {
                LL x1 = (x % b + b) % b;
                if (!x1) x1 = b;
                LL x2 = (y % a + a) % a;
                if (!x2) x2 = a;
                printf("%lld %lld\n", x1, x2);
            }
        }
    }
    return 0;
}