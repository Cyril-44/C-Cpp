#include <bits/stdc++.h>
using namespace std;
constexpr int N = 20, SUM[10] = {0, 1, 3, 6, 10, 15, 21, 28, 36, 45};
using i64 = long long;
int calc1(i64 x, int k) {
    static int sta[25];
    sta[sta[0] = 1] = x % 10;
    while (x /= 10) sta[++sta[0]] = x % 10;
    int res = 0;
    while (sta[0] && k--) res += sta[sta[0]--];
    return res;
}
i64 calc2(i64 N) {
    if (N <= 0) return 0;
    i64 res = 0;
    i64 base = 1;
    while (base <= N) {
        i64 l = N / (base * 10);
        i64 m = (N / base) % 10;
        i64 r = N % base;
        res += l * SUM[9] * base;
        res += (m * (m - 1ll) >> 1) * base;
        res += m * (r + 1ll);
        base *= 10;
    }
    return res;
}
/* i64 calc2(i64 x, i64 y) {
    i64 res = 0;
    for (i64 base = 1, back = 0; y; x /= 10, y /= 10, base *= 10) {
        res += (y - x) / 10 * SUM[9] + (y % 10) * (back + 1);
        if (y % 10 > 0) res += SUM[y % 10 - 1] * base;
        back += y % 10 * base;
    }
    return res;
} */
int main() {
    int t;
    i64 k;
    scanf("%d", &t);
    while (t--) {
        scanf("%lld", &k);
        int digits = 1;
        i64 base = 1, ans = 0;
        for (i64 add = 9; k >= add * digits; ++digits, add *= 10) {
            ans += calc2(base * 10 - 1) - calc2(base - 1);
            k -= add * digits, base *= 10;
        }
        i64 last = k / digits;
        if (last > 0) ans += calc2(base + last - 1) - calc2(base - 1);
        if (k % digits) ans += calc1(base + last, k % digits);
        printf("%lld\n", ans);
    }
    return 0;
}