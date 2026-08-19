#include <cstdio>
#include <algorithm>
constexpr int N = 100005;
constexpr int64_t INF = 1ll << 62;
std::pair<int64_t,int> b[N];
int64_t a[N];
int n;
class Seg {
    int64_t mx[N], add[N];
    int L, R, P; int64_t X;
    void pushup(int u) {
        mx[u] = std::max(mx[u<<1], mx[u<<1|1]);
    }
    void pushdown(int u) {
        if (add[u]) {
            mx[u<<1] += add[u], mx[u<<1|1] += add[u];
            add[u<<1] += add[u], add[u<<1|1] += add[u];
            add[u] = 0;
        }
    }
    void upd(int u, int l, int r) {
        if (L <= l && r <= R) { mx[u] += X, add[u] += X; return; }
        int mid = l + r >> 1;
        pushdown(u);
        if (L <= mid) upd(u<<1, l, mid);
        if (mid < R) upd(u<<1|1, mid+1, r);
        pushup(u);
    }
    void set(int u, int l, int r) {
        if (l == r) { mx[u] = X; return; }
        int mid = l + r >> 1;
        pushdown(u);
        if (P <= mid) set(u<<1, l, mid);
        if (mid < P) set(u<<1|1, mid+1, r);
        pushup(u);
    }
    int64_t inq(int u, int l, int r) {
        if (L <= l && r <= R) return mx[u];
        int mid = l + r >> 1;
        pushdown(u); int64_t res = -INF;
        if (L <= mid) res = std::max(res, inq(u<<1, l, mid));
        if (mid < R) res = std::max(res, inq(u<<1|1, mid+1, r));
        return res;
    }
public:
    int64_t inquire(int l, int r) { L=l, R=r; return inq(1, 1, n); }
    void rangeadd(int l, int r, int x) { L=l, R=r, X=x; upd(1, 1, n); }
    void erase(int pos) { P=pos, X=-INF; set(1, 1, n); }
    void build(int u=1, int l=1, int r=n) {
        add[u] = 0;
        if (l == r) { mx[u] = a[l] + l; return; }
        int mid = l + r >> 1;
        build(u<<1, l, mid); build(u<<1|1, mid+1, r);
        pushup(u);
    }
} f;
int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++) {
            scanf("%lld", &a[i]);
            b[i] = {a[i] - i, i};
        }
        f.build();
        std::sort(b+1, b+1+n);
        bool noSol = false;
        for (int i = 2; i <= n; i++)
            if (b[i].first == b[i-1].first)
                { noSol = true; break; }
        if (noSol) { puts("-1"); continue; }
        int64_t ans = 0;
        for (int i = 1; i <= n; i++) {
            int64_t res = b[i].second < n ? f.inquire(b[i].second + 1, n) : -INF;
            ans = std::max(ans, b[i].first + res - 1);
            f.erase(b[i].second);
            if (b[i].second < n) f.rangeadd(b[i].second+1, n, -2);
        }
        printf("%lld\n", ans);
    }
    return 0;
}