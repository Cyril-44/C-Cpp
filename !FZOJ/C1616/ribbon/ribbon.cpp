#include <bits/stdc++.h>
constexpr int N = 200005, MOD = 998244353;
#ifdef CLANGD
#define N 25
#endif
int n;
template<class Comp = std::less<int>>
inline void calc(const int *a, int *b, const bool inv = false) { // b[i] 记录前面/后面第一个比 a[i] 小/大的位置
    static int sta[N];
    int top = 0;
    auto solveOnce = [&](int i) {
        while (top && !Comp{}(a[sta[top]], a[i])) --top;
        b[i] = sta[top], sta[++top] = i;
    };
    if (inv) for (int i = n; i >= 1; i--) solveOnce(i);
    else     for (int i = 1; i <= n; i++) solveOnce(i);
}
struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    Mint(int v=0) : val(v) {}
private: int val;
} g[N];
struct MinBIT {
    int tr[N];
    void clear() { memset(tr, 0x3f, sizeof(int) * (n+1)); }
    void upd(int p, int x) { for (; p <= n; p += p & -p) tr[p] = std::min(tr[p], x); }
    int min(int p) { int res = 0x3f3f3f3f; for (; p > 0; p -= p & -p) res = std::min(res, tr[p]); return res; }
} fmnbit;
int p[N], c[N], fpgt[N], fplt[N], bpgt[N], bplt[N], d[N];
std::vector<int> id21[N], id43[N], id34[N], id12[N];
int64_t f[N];
struct MinSegTr {
    int64_t tr[N<<2], L, R, P, X;
    void clear() { memset(tr, 0x3f, sizeof tr); }
    void upd(int u, int l, int r) {
        if (L <= l && r <= R) { tr[u] = std::min(tr[u], X); return; }
        int mid = l + r >> 1;
        if (L <= mid) upd(u<<1, l, mid);
        if (mid < R) upd(u<<1|1, mid+1, r);
    }
    int64_t inq(int u, int l, int r) {
        if (l == r) return tr[u];
        int mid = l + r >> 1;
        return std::min(tr[u], P <= mid ? inq(u<<1, l, mid) : inq(u<<1|1, mid+1, r));
    }
    void update(int l, int r, int64_t x) { L=l, R=r, X=x, upd(1, 1, n+1); }
    int64_t inquire(int p) { P=p; return inq(1, 1, n+1); }
    void extract(int u=1, int l=1, int r=n+1, int64_t val=1ll<<60) {
        val = std::min(val, tr[u]);
        if (l == r) { f[l] = val; return; }
        int mid = l + r >> 1;
        extract(u<<1, l, mid, val), extract(u<<1|1, mid+1, r, val);
    }
} fmnseg;
std::vector<std::pair<int64_t, Mint>> sub[N];
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &p[i]);
    for (int i = 1; i <= n; i++) scanf("%d", &c[i]);
    calc(p, fplt);
    calc(p, bplt, 1);
    calc<std::greater<int>>(p, fpgt);
    calc<std::greater<int>>(p, bpgt, 1);
    for (int i = 1; i <= n; i++) {
        id21[bplt[i]].push_back(i);
        id43[fpgt[i]].push_back(i);
        id34[bpgt[i]].push_back(i);
        id12[fplt[i]].push_back(i);
    }
    memset(d, 0x3f, sizeof(int) * (n+1));
    fmnbit.clear();
    for (int i = n; i >= 1; i--) { // 2143 
        for (int j : id21[i]) d[j] = std::min(d[j], fmnbit.min(n - p[j] + 1));
        for (int j : id43[i]) fmnbit.upd(n - p[j] + 1, j);
    }
    fmnbit.clear();
    for (int i = n; i >= 1; i--) { // 3412 
        for (int j : id34[i]) d[j] = std::min(d[j], fmnbit.min(p[j]));
        for (int j : id12[i]) fmnbit.upd(p[j], j);
    }
    d[n+1] = n+1;
    for (int i = n; i >= 1; i--) d[i] = std::min(d[i], d[i+1]);
    fmnseg.clear(), fmnseg.update(1, 1, 0);
    for (int i = 1; i <= n; i++) {
        fmnseg.update(i+1, d[i], fmnseg.inquire(i) + c[i]);
    }
    fmnseg.extract();
    std::map<int64_t, Mint> fmp;
    fmp[0] = 1, sub[1].emplace_back(0, 1);
    for (int i = 1; i <= n+1; i++) {
        g[i] = fmp[f[i]];
        fmp[f[i] + c[i]] += g[i];
        sub[d[i]].emplace_back(f[i] + c[i], g[i]);
        for (const auto& [v, x] : sub[i])
            fmp[v] -= x;
    }
    printf("%lld %d\n", f[n+1], g[n+1]);
}