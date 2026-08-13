#include <bits/stdc++.h>
constexpr int N = 300005, M = 1000005;
int n, a[N], pos[N];
std::vector<std::pair<int,int>> pnts[N], qrys[N];
struct BIT {
    int64_t tr[N];
    void upd(int p, int x) { for (; p <= n; p += p & -p) tr[p] += x; }
    int64_t sum(int p) { int64_t res = 0; for (; p > 0; p -= p & -p) res += tr[p]; return res; }
} fs;
int64_t ans[M];
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]), pos[a[i]] = i;
    for (int k = 2; (1<<k) <= n*2; k++) {
        int s = 1<<k;
        for (int j = 1; j < s; j++) {
            int i = s - j;
            if (std::__gcd(i, j) == 1)
                for (int m = 1; ; m++) {
                    int mi = m * i, mj = m * j;
                    if (mi > n || mj > n) break;
                    if (pos[mi] < pos[mj]) pnts[pos[mj]].emplace_back(pos[mi], k);
                    // printf("(%d,%d) += %d\n", pos[mi], pos[mj], k);
                }
        }
    }
    int m;
    scanf("%d", &m);
    for (int i = 1; i <= m; i++) {
        int l, r;
        scanf("%d%d", &l, &r);
        qrys[r].emplace_back(l, i);
    }
    int64_t tot = 0;
    for (int r = 1; r <= n; r++) {
        for (const auto &[l, v] : pnts[r])
            fs.upd(l, v), tot += v;
        for (const auto &[l, id] : qrys[r])
            ans[id] = tot - fs.sum(l-1);
    }
    for (int i = 1; i <= m; i++)
        printf("%lld\n", ans[i]);
    return 0;
}