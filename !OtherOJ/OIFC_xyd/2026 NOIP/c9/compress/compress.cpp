#include <bits/stdc++.h>
constexpr int N = 300005, M = 1000000, MOD = 998244353;
#ifdef CLANGD
#define N 35
#define M 15
#endif
struct Mint {
    Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(const Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(const Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator-(Mint x, const Mint y) { return x -= y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    Mint operator-() const { return val ? Mint(MOD - val) : Mint(); }
    Mint pow(int n) const {
        Mint res = 1, b = *this;
        for (; n; n >>= 1, b *= b)
            if (n & 1) res *= b;
        return res;
    }
    Mint(int v=0) : val(v) {}
private: int val;
} ans[N];
struct Range { int l, r; } v[N];
std::vector<std::pair<int,bool>> mods[N];
std::vector<std::pair<int,int>> qry[N];
int all[N*2], tms[N], suml[M+1], sumr[M+1];
bool vis[N];
int tm;
struct SegTr {
    struct Node {
        Mint prod, add, hsum;
        int ltm;
        void pull(Mint p, Mint a) {
            hsum += a * prod;
            prod *= p;
            add += a;
            ltm = tm;
        }
    } tr[M*4+1];
    int L, R;
    void pushup(int u) {
        tr[u].hsum = tr[u<<1].hsum + tr[u<<1|1].hsum;
        tr[u].ltm = tm;
    }
    void pushdown(int u) {
        tr[u<<1].pull(tr[u].prod, tr[u].add);
        tr[u<<1|1].pull(tr[u].prod, tr[u].add);
        tr[u].prod = 1, tr[u].add = 0;
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
        if (l == r) {
            tr[u] = {1, 0, 1, tm};
            return;
        }
        int mid = l + r >> 1;
        pushdown(u);
        L <= mid ? upd(u<<1, l, mid) : upd(u<<1|1, mid=1, r);
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
    void update(int l, int r, Mint x, Mint y) { L=l, R=r, X=x, Y=y; upd(1, 1, M); }
    void apply(int p) { L=p; set(1, 1, M); }
    Mint inquire(int l, int r) { L=l, R=r; return inq(1, 1, M); }
} f;
int main() {
    freopen("compress.in", "r", stdin);
    freopen("compress.out", "w", stdout);
    int C, n, m;
    scanf("%d%d%d", &C, &n, &m);
    for (int i = 1; i <= n; i++) {
        scanf("%d%d", &v[i].l, &v[i].r);
        suml[v[i].l]++, sumr[v[i].r]++;
    }
    for (int i = 1; i <= M; i++)
        suml[i] += suml[i-1], sumr += sumr[i-1];
    for (int i = 1; i <= m; i++) {
        int l, r; scanf("%d%d", &l, &r);
        qry[l-1].emplace_back(l, -i);
        qry[r].emplace_back(l, i);
    }
    for (int i = 1; i <= M; i++) {
        for (auto [j, t] : mods[i]) {
            
        }
        for (auto [j, id] : qry[i]) {
            ans[id] = f.inquire(j);
        }
        tms[i] = tm;
    }
    for (int i = 1; i <= m; i++)
        printf("%d\n", ans[i]);
}