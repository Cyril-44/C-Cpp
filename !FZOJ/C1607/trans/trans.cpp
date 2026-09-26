#include <cstdio>
#include <algorithm>
#include <cstdint>
#include <limits>
constexpr int N = 500004;
int a[N], b[N];
int64_t bs1[N], bs2[N], bs[N];
int main() {
    int n, m, q;
    scanf("%d%d%d", &n, &m, &q);
    int64_t asum = 0;
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        if (i & 1) asum += a[i];
        else asum -= a[i];
    }
    for (int j = 1; j <= m; j++) {
        scanf("%d", &b[j]);
        if (j & 1) b[j] = -b[j];
        bs1[j] = bs1[j-1] + b[j], bs2[j] = bs2[j-1] - b[j];
    }
    int64_t bmin = std::numeric_limits<int64_t>::max(), bmax = std::numeric_limits<int64_t>::min();
    for (int j = 0; j <= m-n; j++)
        bs[j] = (j & 1) ? bs2[n+j] - bs2[j] : bs1[n+j] - bs1[j];
    std::sort(bs, bs+m-n+1);
    auto print = [&]() {
        int id = std::lower_bound(bs, bs+m-n+1, -asum)-bs;
        int64_t ans = std::numeric_limits<int64_t>::max();
        if (id <= m-n) ans = std::min(ans, asum + bs[id]);
        if (id > 0) ans = std::min(ans, -(asum + bs[id-1]));
        printf("%lld\n", ans);
    };
    print();
    for (int l, r, v; q--; ) {
        scanf("%d%d%d", &l, &r, &v);
        if (l%2==r%2) {
            if (l%2) asum += v;
            else asum -= v;
        }
        print();
    }
    return 0;
}