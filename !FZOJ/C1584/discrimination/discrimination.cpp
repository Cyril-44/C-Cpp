#include <cstdio>
using LL = long long;
inline int calc(LL v) {
    int cnt = 0;
    for (LL i = 1; i <= v; i = (v / (v / i)) + 1)
        ++cnt;
    return cnt;
}
inline LL min(LL x, LL y) { return x<y?x:y; }
inline LL max(LL x, LL y) { return x>y?x:y; }
inline void out(__int128 num) {
    static int sta[45];
    sta[sta[0] = 1] = num % 10;
    while (num /= 10) sta[++sta[0]] = num % 10;
    while (sta[0]) putchar(sta[sta[0]--] | '0');
    putchar('\n');
}
int main() {
    int T; LL n, m, r;
    scanf("%d", &T);
    while (T--) {
        scanf("%lld%lld%lld", &n, &m, &r);
        if (r == 0) r += m;
        LL rgl = 1, rgr = 1;
        __int128 sum = 0;
        for (int v = 1; rgl <= n; ++v, rgl=rgr+1, rgr+=v/2+1) {
            if (rgr < r) continue;
            LL R = (min(n, rgr)-r)/m, L = max((rgl-r+m-1)/m, 0);
            sum += (R - L + 1) * v;
            // fprintf(stderr, "%d: [%lld, %lld] %lld~%lld\n", v, rgl, rgr, L, R);
        }
        out(sum);
    }
    return 0;
}