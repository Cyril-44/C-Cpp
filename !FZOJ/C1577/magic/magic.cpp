#include <cstdio>
#include <algorithm>
#include <cstring>
constexpr int N = 200005;
int n, a[N];
struct Seg {
    long long sum[N << 2], add[N << 2];
    int L, R, X;
    void build(int u=1, int l=1, int r=n) {
        if (l == r) sum[u] = a[l];
        else {
            int mid = l + r >> 1;
            build(u<<1, l, mid);
            build(u<<1|1, mid+1, r);
            pushup(u);
        }
    }
    void pushdown(int u, int ls, int rs) {
        if (add[u]) {
            sum[u<<1] += add[u] * ls, add[u<<1] += add[u];
            sum[u<<1|1] += add[u] * rs, add[u<<1|1] += add[u];
            add[u] = 0;
        }
    }
    void pushup(int u) {
        sum[u] = sum[u<<1] + sum[u<<1|1];
    }
    void upd(int u, int l, int r) {
        if (L <= l && r <= R) sum[u] += X*(r-l+1ll), add[u] += X;
        else {
            int mid = l + r >> 1;
            pushdown(u, mid-l+1, r-mid);
            if (L <= mid) upd(u<<1, l, mid);
            if (mid < R) upd(u<<1|1, mid+1, r);
            pushup(u);
        }
    }
    long long inq(int u, int l, int r) {
        if (L <= l && r <= R) return sum[u];
        int mid = l + r >> 1;
        pushdown(u, mid-l+1, r-mid);
        long long res = 0;
        if (L <= mid) res += inq(u<<1, l, mid);
        if (mid < R) res += inq(u<<1|1, mid+1, r);
        return res;
    }
    void update(int l, int r, int x) {
        L=l, R=r, X=x;
        upd(1, 1, n);
    }
    long long inquire(int l, int r) {
        L=l, R=r;
        return inq(1, 1, n);
    }
    long long inquire(int pos) { return inquire(pos,pos); }
} fs;
namespace BF {
    constexpr int N = 105, P = 1000;
    long long a[N];
    double f[2][P+1];
    inline void work(int l, int r) {
        for (int i = l; i <= r; i++) a[i] = fs.inquire(i);
        for (int p = 0; p <= P; p++) f[l&1][p] = a[l] * p *1./P;
        for (int i = l+1; i < r; i++) {
            double curr = f[i&1^1][0];
            for (int p = P; p >= 0; p--) {
                curr = std::max(curr, f[i&1^1][P-p]);
                f[i&1][p] = curr + a[i] * p *1./P;
            }
        }
    }
}
int main() {
    int q;
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    fs.build();
    for (int op, l, r, v; q--; ) {
        scanf("%d%d%d", &op, &l, &r);
        if (op == 1) {
            scanf("%d", &v);
            fs.update(l, r, v);
        } else {
            if (l == r) printf("%.3lf\n", fs.inquire(l) / 2.);
            else if (r - l == 1) printf("%.3lf\n", (double)std::min(fs.inquire(l), fs.inquire(r)));
            // else if (n <= 100) BF::work(l, r);
        }
    }
    return 0;
}