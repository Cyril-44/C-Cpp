#include <stdio.h>
inline void umax(int &x, const int &y) {
    x = x > y ? x : y;
}
int main() {
#ifndef ONLINE_JUDGE
    freopen("bamboo.in", "r", stdin);
    freopen("bamboo.out", "w", stdout);
#endif
    int n, ai, bi, l=0, r=0;
    auto calc = [&] (const int &a, const int &b) -> int {
        int res = 0;
        if (a > l) res += a - l;
        if (b > r) res += b - r;
        return res;
    };
    scanf("%d", &n);
    while (n--) {
        scanf("%d%d", &ai, &bi);
        ai -= bi;
        if (calc(ai, bi) > calc(bi, ai)) {
            umax(r, bi);
            umax(l, ai);
        }
        else {
            umax(l, bi);
            umax(r, ai);
        }
    }
    printf("%d\n", l + r);
    return 0;
}