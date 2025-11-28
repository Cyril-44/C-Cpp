#include <cstdio>
#include <cmath>
#include <algorithm>
const int N = 50005;
int c[N], h[N], blksz;
struct Query {
    int l, r, id;
    inline bool operator<(const Query &x) const {
        return (l / blksz ^ x.l / blksz) ? (l < x.l) : (
            (l / blksz & 1) ? (r < x.r) : (r > x.r)
        );
    }
} q[N];
long long sum, ans[N][2];
inline void add(const int &x) {
    sum += h[x]; // h_x+1C2 - h_xC2 = h_x
    ++h[x];
}
inline void del(const int &x) {
    --h[x];
    sum -= h[x]; // h_xC2 - h_x-1C2 = h_x-1
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    blksz = sqrt(n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &c[i]);
    for (int i = 0; i < m; i++) {
        scanf("%d%d", &q[i].l, &q[i].r);
        q[i].id = i;
    }
    std::sort(q, q + m);
    for (int i = 0, l = 1, r = 0; i < m; i++) {
        if (q[i].l ^ q[i].r) {
            while (l > q[i].l) add(c[--l]); // 加上l-1
            while (r < q[i].r) add(c[++r]); // 加上r+1
            while (r > q[i].r) del(c[r--]); // 减去当前r
            while (l < q[i].l) del(c[l++]); // 减去当前l
            ans[q[i].id][0] = sum;
            ans[q[i].id][1] = (q[i].r - q[i].l + 1ll) * (q[i].r - q[i].l) / 2;
        }
        else ans[q[i].id][1] = 1;
    }
    long long gcd;
    for (int i = 0; i < m; i++) {
        if (ans[i][0]) {
            gcd = std::__gcd(ans[i][0], ans[i][1]);
            ans[i][0] /= gcd, ans[i][1] /= gcd;
        }
        else ans[i][1] = 1;
        printf("%lld/%lld\n", ans[i][0], ans[i][1]);
    }
    return 0;
}