#include <stdio.h>
#include <algorithm>
using std::min;
using std::max;
const int N = 200005, MOD = 1000000007;
struct Node {
    int p, m, lmt;
    inline bool operator< (const Node &x) const {
        return lmt < x.lmt;
    }
} a[N];
int pow2[N];
int main() {
#ifndef ONLINE_JUDGE
    freopen("hole.in", "r", stdin);
    freopen("hole.out", "w", stdout);
#endif
    int n, ans, mx;
    scanf("%d", &n);
    pow2[0] = 1;
    for (int i = 1; i <= n; i++)
        pow2[i] = (pow2[i-1] << 1) % MOD;
    for (int i = 0; i < n; i++)
        scanf("%d", &a[i].m);
    mx = MOD;
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i].p);
        a[i].lmt = min(a[i].m - a[i].p, a[i].p - 1);
        mx = min(mx, max(a[i].m - a[i].p, a[i].p - 1));
    }
    std::sort(a, a + n);
    for (int i = 0; i < n; i++)
        if (a[i].lmt > mx)
            a[i].lmt = mx;
    ans = (1ll + (a->lmt) * 1ll * pow2[n]) % MOD;
    for (int i = 1; i < n; i++) {
        ans = (ans + (a[i].lmt - a[i-1].lmt) * 1ll * pow2[n - i]) % MOD;
    }
    ans = (ans + (mx - a[n-1].lmt)) % MOD;
    printf("%d\n", ans);
    return 0;
}