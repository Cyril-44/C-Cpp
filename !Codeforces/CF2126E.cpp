#include <cstdio>
const int N = 100005;
int a[N], b[N];
typedef long long LL;
inline LL gcd(LL x, LL y) {
    LL z;
    while (y) z = x, x = y, y = z % y;
    return x;
}
inline long long lcm(long long x, long long y) {
    return x / gcd(x, y) * 1ll * y;
}
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++) scanf("%d", a+i);
        for (int i = 1; i <= n; i++) scanf("%d", b+i);
        if (a[n] != b[1]) { puts("NO"); continue; }
        bool flg = true;
        long long lst = 1;
        for (int i = 1; i <= n; i++) {
            if (lcm(lst, b[i]) != b[i]) {
                flg = false; break;
            }
            lst = lcm(lst, b[i]);
        }
        lst = 1;
        for (int i = n; i >= 1; i--) {
            if (lcm(lst, a[i]) != a[i]) {
                flg = false; break;
            }
            lst = lcm(lst, a[i]);
        }
        lst = 0;
        for (int i = 1; i <= n; i++)
            lst = gcd(lst, gcd(a[i], b[i]));
        if (lst != b[1])
            flg = false;
        for (int i = 0; i < n; i++) {
            if (gcd(a[i], b[i+1]) != b[1]) {
                flg = false; break;
            }
        }
        puts(flg ? "YES" : "NO");
    }
    return 0;
}