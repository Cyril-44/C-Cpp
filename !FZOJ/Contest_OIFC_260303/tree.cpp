#include <stdio.h>
#include <set>
constexpr int N = 18;
using LL = long long;
int a[1 << N];
template<class T> inline void umin(T &x, T y) { x = std::min(x, y); }
struct Node {
    /* Lower Convex
       Maintain yi = (k0 + 2i)xi + bi And Its Lowest Point x-axis Saved in "x" */
    LL x0{}, y0{}; // Save (x0, y0) to calculate each bi
    int k0{}; // In fact k0 = -x.size()
    std::multiset<LL> x;
    inline void insert(int a) { // Global Plus y = |x - a|
        if (x.empty()) x0 = a;
        else if (a < x0) y0 += -k0 * (x0 - a), x0 = a;
        else y0 += a - x0;
        x.insert(a); --k0;
    }
    inline LL operator()() const {
        LL cur = y0, ans = y0, k = k0;
        for (auto it = ++x.begin(); it != x.end(); ++it)
            umin(ans, cur += (k += 2) * (*it - *std::prev(it)));
        return ans;
    }
} tr[1 << N];
Node operator+(const Node &m, const Node &n) { // Do Min+ Convolution On m and n
    Node ret{m.x0+n.x0, m.y0+n.y0, m.k0};
    auto itm = m.x.begin(), itn = n.x.begin();
    while (itm != m.x.end()) ret.x.insert(ret.x.end(), *itm++ + *itn++);
    return ret;
}
int main() {
    freopen("tree.in", "r", stdin);
    freopen("tree.out", "w", stdout);
    int n, q;
    scanf("%d", &n);
    for (int i = 1; !(i >> n); i++)
        scanf("%d", &a[i]);
    for (int i = 1 << n-1; !(i >> n); i++)
        tr[i].insert(a[i]);
    for (int i = (1 << n-1) - 1; i; i--)
        (tr[i] = tr[i<<1] + tr[i<<1|1]).insert(a[i]);
    scanf("%d", &q);
    for (int u, x; q--; ) {
        scanf("%d%d", &u, &x);
        a[u] = x;
        if (u >> n-1) (tr[u] = Node{}).insert(a[u]), u >>= 1;
        for (; u; u >>= 1) (tr[u] = tr[u<<1] + tr[u<<1|1]).insert(a[u]);
        printf("%lld\n", tr[1]());
    }
}