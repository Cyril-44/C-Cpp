#include <stdio.h>
#include <algorithm>
#include <queue>
#include <functional>
const int N = 100005;
struct Seg {
    int l, r;
    inline bool operator>(const Seg &x) const {
        return l < x.l || l == x.l && r < x.r;
    }
    inline bool operator<(const Seg &x) const {
        return r > x.r;
    }
} a[N];
int main() {
    int t, n, d, k, mx, mn, mxid, mnid, r;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d%d", &n, &d, &k);
        for (int i = 0; i < k; i++)
            scanf("%d%d", &a[i].l, &a[i].r);
        mn = k+1, mx = 0, r = 0;
        std::sort(a, a + k, std::greater<Seg>());
        std::priority_queue<Seg> q;
        for (Seg cur = {1, d}; cur.r <= n; ++cur.l, ++cur.r) {
            while (r < k && a[r].l <= cur.r) q.push(a[r++]);
            while (!q.empty() && q.top().r < cur.l) q.pop();
            if (mx == 0 || mx < q.size()) {
                mx = q.size();
                mxid = cur.l;
            }
            if (mn > q.size()) {
                mn = q.size();
                mnid = cur.l;
            }
        }
        printf("%d %d\n", mxid, mnid);
    }
    return 0;
}