#include <stdio.h>
#include <string.h>
#include <algorithm>
constexpr int N = 100005;
int a[N], p[N];
int n;
namespace BIT {
    int tr[N << 1];
    inline void init() { memset(tr, 0, sizeof tr); }
    inline void upd(int p, int x) {
        for (; p <= (n<<1); p += p & -p) tr[p] += x;
    }
    inline int sum(int p) {
        int res = 0;
        for (; p > 0; p -= p & -p) res += tr[p];
        return res;
    }
};
int main() {
    freopen("snow.in", "r", stdin);
    freopen("snow.out", "w", stdout);
#ifndef ONLINE_JUDGE
    // freopen("snow.txt", "w", stderr);
#endif
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]), p[a[i]] = i;
    int common = 0;
    for (int i = 1; i <= n; i++)
        common = std::__gcd(common, std::abs(a[i] - a[a[i]]));
    for (int i = 1; i <= n; i++)
        if (common % i == 0) {
            long long ans = 0;
            int size = n / i + 1;
            BIT::init();
            for (int j = 1; j <= n; j++) {
                int grp = a[j] % i, id = (a[j] + i - 1) / i;
                int now = grp * size + id, end = (grp + 1) * size;
                // fprintf(stderr, "%d %d\n", now, end);
                ans += BIT::sum(end) - BIT::sum(now);
                BIT::upd(now, 1);
            }
            printf("%lld\n", ans);
        }
        else puts("-1");
    return 0;
}
