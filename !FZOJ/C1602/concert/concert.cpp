#include <cstdio>
#include <algorithm>
constexpr int N = 100004;
using LD = long double;
int x[N], p[N], n;
LD psum[N];
class SegTr {
    struct Node {
        LD mn, mx, val;
        friend Node operator+(const Node &x, const Node &y) {
            return Node{
                std::min(x.mn, y.mn), 
                std::max(x.mx, y.mx), 
                std::max({x.val, y.val, y.mx - x.mn})
            };
        }
    } tr[N << 2];
    int L, R;    
    Node inq(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u];
        int mid = l + r >> 1;
        if (R <= mid) return inq(u<<1, l, mid);
        if (mid < L) return inq(u<<1|1, mid+1, r);
        return inq(u<<1, l, mid) + inq(u<<1|1, mid+1, r);
    }
public:
    void build(int u=1, int l=1, int r=n) {
        if (l == r) { tr[u] = {psum[l], psum[l], 0.}; return; }
        int mid = l + r >> 1;
        build(u<<1, l, mid), build(u<<1|1, mid+1, r);
        tr[u] = tr[u<<1] + tr[u<<1|1];
    }
    LD inquire(int l, int r) { L=l, R=r; return inq(1, 1, n).val; }
} f;
int main() {
    int m, c;
    scanf("%d%d%d", &n, &m, &c);
    for (int i = 1; i <= n; i++) scanf("%d", &x[i]);
    for (int i = 2; i <= n; i++) scanf("%d", &p[i]), psum[i] = psum[i-1] + p[i] / (LD)100.;
    for (int i = 1; i <= n; i++) psum[i] = x[i] / 2. - psum[i] * c;
    LD ans = 0;
    f.build();
    for (int l, r; m--; ) {
        scanf("%d%d", &l, &r);
        ans += f.inquire(l, r);
        // fprintf(stderr, "[%d,%d]: += %g\n", l, r, f.inquire(l, r));
    }
    printf("%.9Lf\n", ans);
    return 0;
}