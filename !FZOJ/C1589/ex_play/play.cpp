#include <bits/stdc++.h>
constexpr int N = 100005, MOD = (int)1e9 + 7;
int n, a[N];
struct Mint {
    template<typename T> explicit operator T() const { return static_cast<T>(val); }
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint a, Mint b) { return a += b; }
    friend Mint operator-(Mint a, Mint b) { return a -= b; }
    friend Mint operator*(Mint a, Mint b) { return a *= b; }
    Mint(int o = 0) : val(o) {}
private: int val;
};
Mint sqsum[N];
int sqval[N];
class SegTr {
    struct Node {
        Mint psum, sum;
        int add, cert;
        void pull(int v) {
            add += v, cert -= v;
            sum += psum * v;
        }
    } tr[N << 2];
    void pushup(int u) {
        tr[u].psum = tr[u<<1].psum + tr[u<<1|1].psum;
        tr[u].sum = tr[u<<1].sum + tr[u<<1|1].sum;
        tr[u].cert = std::min(tr[u<<1].cert, tr[u<<1|1].cert);
    }
    void pushdown(int u) {
        tr[u<<1].pull(tr[u].add);
        tr[u<<1|1].pull(tr[u].add);
        tr[u].add = 0;
    }
    int L, R;
    void upd(int u, int l, int r) {
        if (L <= l && r <= R && tr[u].cert) tr[u].pull(1);
        else if (l == r) tr[u].sum += tr[u].psum += 1, tr[u].cert = int((tr[u].psum+1)*(tr[u].psum+1) - tr[u].psum*tr[u].psum - 1);
        else {
            int mid = l + r >> 1;
            pushdown(u);
            if (L <= mid) upd(u<<1, l, mid);
            if (mid < R) upd(u<<1|1, mid+1, r);
            pushup(u);
        }
    }
    Mint inq(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].sum;
        int mid = l + r >> 1;
        pushdown(u);
        Mint res;
        if (L <= mid) res += inq(u<<1, l, mid);
        if (mid < R) res += inq(u<<1|1, mid+1, r);
        return res;
    }
public:
    void build(int u=1, int l=1, int r=n) {
        if (l == r) {
            int idx = std::upper_bound(sqval+1, sqval+sqval[0]+1, a[l]) - sqval - 1;
            tr[u].psum = idx;
            tr[u].sum = sqsum[idx] + Mint(a[l] - sqval[idx]) * idx;
            tr[u].cert = (idx+1)*(idx+1) - a[l] - 1;
        } else {
            int mid = l + r >> 1;
            build(u<<1, l, mid);
            build(u<<1|1, mid+1, r);
            pushup(u);
        }
    }
    void update(int l, int r) { L=l, R=r; upd(1,1,n); }
    Mint inquire(int l, int r) { L=l, R=r; return inq(1,1,n); }
    void debug(int u=1, int l=1, int r=n) {
        if (l == r) printf("%d: %d %d %d\n", l, tr[u].psum, tr[u].sum, tr[u].cert);
        else {
            int mid = l + r >> 1;
            pushdown(u);
            debug(u<<1, l, mid);
            debug(u<<1|1, mid+1, r);
        }
    }
};
static SegTr f;
int main() {
    int m;
    scanf("%d%d", &n, &m);
    for (int i = sqval[0] = 1; ; i++, sqval[0]++) {
        sqval[i] = i*i;
        if (sqval[i] > (int)1e9) break;
        sqsum[i] = sqsum[i-1] + Mint(sqval[i] - sqval[i-1]) * (i-1) + 1;
    }
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    f.build();
    while (m--) {
        int op, l, r;
        scanf("%d%d%d", &op, &l, &r);
        if (op == 1) f.update(l, r);
        else printf("%d\n", f.inquire(l, r));
    }
    return 0;
}