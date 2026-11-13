#include <bits/stdc++.h>
constexpr int N = 300005, MOD = 998244353;
#ifdef CLANGD
#define N 35
#endif
struct Mint {
    constexpr Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    constexpr Mint& operator-=(const Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    constexpr Mint& operator*=(const Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    constexpr friend Mint operator+(Mint x, const Mint y) { return x += y; }
    constexpr friend Mint operator-(Mint x, const Mint y) { return x -= y; }
    constexpr friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    constexpr Mint operator-() const { return val ? Mint(MOD - val) : Mint(); }
    constexpr Mint pow(int n) const {
        Mint res = 1, b = *this;
        for (; n; n >>= 1, b *= b)
            if (n & 1) res *= b;
        return res;
    }
    constexpr Mint(int v=0) : val(v) {}
    template<typename T> constexpr explicit operator T() const { return static_cast<T>(val); }
private: int val;
} ans[N];
constexpr Mint onehalf = Mint(2).pow(MOD-2);
using Pii = std::pair<int,int>;
struct Range { int l, r; } v[N];
Pii pnts[N*2];
std::vector<Pii> qry[N*2];
int all[N*2], suml[N*2], sumr[N*2];
int n;
struct SegTr {
    struct Node {
        Mint sum, prodtag = 1, hsum, haddtag;
        void pull(Mint p, Mint a) {
            hsum += sum * a, haddtag += a * prodtag;
            sum *= p, prodtag *= p;
        }
    } tr[N << 3];
    int L, R;
    void pushup(int u) {
        tr[u].sum = tr[u<<1].sum + tr[u<<1|1].sum;
        tr[u].hsum = tr[u<<1].hsum + tr[u<<1|1].hsum;
    }
    void pushdown(int u) {
        tr[u<<1].pull(tr[u].prodtag, tr[u].haddtag);
        tr[u<<1|1].pull(tr[u].prodtag, tr[u].haddtag);
        tr[u].prodtag = 1, tr[u].haddtag = 0;
    }
    Mint X, Y;
    void upd(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].pull(X, Y);
        int mid = l + r >> 1;
        pushdown(u);
        if (L <= mid) upd(u<<1, l, mid);
        if (mid < R) upd(u<<1|1, mid+1, r);
        pushup(u);
    }
    void set(int u, int l, int r) {
        if (l == r) { tr[u] = {1, 1, 0, 0}; return; }
        int mid = l + r >> 1;
        pushdown(u);
        L <= mid ? set(u<<1, l, mid) : set(u<<1|1, mid+1, r);
        pushup(u);
    }
    Mint inq(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].hsum;
        int mid = l + r >> 1; Mint res = 0;
        pushdown(u);
        if (L <= mid) res += inq(u<<1, l, mid);
        if (mid < R) res += inq(u<<1|1, mid+1, r);
        return res;
    }
    void update(int l, int r, Mint x, Mint y) { L=l, R=r, X=x, Y=y; upd(1, 1, 2*n); }
    void apply(int p) { L=p; set(1, 1, 2*n); }
    Mint inquire(int l, int r) { L=l, R=r; return inq(1, 1, 2*n); }
} f;
int main() {
    freopen("compress.in", "r", stdin);
    freopen("compress.out", "w", stdout);
    int C, m;
    scanf("%d%d%d", &C, &n, &m);
    for (int i = 1; i <= n; i++) {
        scanf("%d%d", &v[i].l, &v[i].r);
        pnts[i*2-1] = {v[i].l, -i};
        pnts[i*2] = {v[i].r, i};
    }
    std::sort(pnts+1, pnts+1+n*2);
    for (int i = 1; i <= 2*n; i++) {
        int id = pnts[i].second;
        if (id < 0) v[-id].l = i, suml[i]++;
        else v[id].r = i, sumr[i]++;
    }
    for (int i = 1; i <= 2*n; i++)
        suml[i] += suml[i-1], sumr[i] += sumr[i-1];
    for (int i = 1; i <= m; i++) {
        int l, r; scanf("%d%d", &l, &r);
        l = std::lower_bound(pnts+1, pnts+1+n*2, Pii(l, -1e9)) - pnts;
        r = std::upper_bound(pnts+1, pnts+1+n*2, Pii(r, 1e9)) - pnts-1;
        qry[r].emplace_back(l, i);
    }
    for (int i = 1; i <= 2*n; i++) {
        { // 扫描线
            int id = pnts[i].second;
            if (id < 0) { // 处理左端点
                f.update(1, 2*n, onehalf, onehalf);
            } else {
                f.apply(i);
                f.update(v[id].l+1, v[id].r, onehalf, 0);
            }
        }
        for (auto [j, id] : qry[i]) {
            Mint pw2 = Mint(2).pow(suml[i] - sumr[j-1]);
            ans[id] = pw2-1 + pw2 * f.inquire(j, i);
        }
    }
    for (int i = 1; i <= m; i++)
        printf("%d\n", ans[i]);
}