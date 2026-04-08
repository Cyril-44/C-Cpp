#include <bits/stdc++.h>
constexpr int N = (int)1e6 + 5, MOD = (int)1e9 + 7;
int a[N], pos[N], n;
struct ModInt {
    inline ModInt(int x = 0) : val(x) {}
    inline explicit operator int() const { return val; }
    inline void fixPos() { if (val >= MOD) val -= MOD; }
    inline void fixNeg() { if (val < 0) val += MOD; }
    inline ModInt& operator+=(ModInt o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    inline ModInt& operator-=(ModInt o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    inline ModInt& operator*=(ModInt o) { val = 1ull * val * o.val % MOD; return *this; }
    inline ModInt operator+(ModInt o) const { return ModInt(*this) += o; }
    inline ModInt operator-(ModInt o) const { return ModInt(*this) -= o; }
    inline ModInt operator*(ModInt o) const { return ModInt(*this) *= o; }
    private: int val;
} inv[N];
struct BIT {
    int tr[N];
    inline void add(int p, int x) {
        for (; p <= n; p += p & -p) tr[p] += x;
    }
    inline int sum(int p) {
        int res = 0;
        for (; p; p -= p & -p) res += tr[p];
        return res;
    }
} f;
inline ModInt binom(int n, int m) {
    ModInt res = 1;
    for (int i = n, j = 1; m--; i--, j++)
        res = res * i * inv[j];
    return res;
}
struct SegTr {
    int tr[N << 2], add[N << 2];
    inline void pushup(int u) {
        tr[u] = std::min(tr[u<<1], tr[u<<1|1]);
    }
    void build(int u=1, int l=1, int r=n+1) {
        if (l == r) tr[u] = l - 1;
        else {
            int mid = l + r >> 1;
            build(u << 1, l, mid);
            build(u << 1 | 1, mid + 1, r);
            pushup(u);
        }
    }
    int ql, qr, x;
    inline void pushdown(int u) {
        if (add[u]) {
            tr[u<<1] += add[u], tr[u<<1|1] += add[u];
            add[u<<1] += add[u], add[u<<1|1] += add[u];
            add[u] = 0;
        }
    }
    void upd(int u, int l, int r) {
        if (ql <= l && r <= qr) tr[u] += x, add[u] += x;
        else {
            int mid = l + r >> 1;
            pushdown(u);
            if (ql <= mid) upd(u << 1, l, mid);
            if (mid < qr) upd(u << 1 | 1, mid + 1, r);
            pushup(u);
        }
    }
    inline void update(int p) {
        if (p >= 1) ql = 1, qr = p, x = 1, upd(1, 1, n+1);
        if (p <= n) ql = p+1, qr = n+1, x = -1, upd(1, 1, n+1);
    }
    inline int inquire() { return tr[1]; }
} g;

int main() {
    inv[1] = 1;
    for (int i = 2; i <= (int)1e6 + 1; i++)
        inv[i] = inv[MOD % i] * (MOD - MOD / i);
    int m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]), pos[a[i]] = i;
    ModInt ans = 0;
    for (int i = n; i >= 1; i--) {
        ans += f.sum(a[i]);
        f.add(a[i], 1);
    }
    ans *= binom(n+m, n);

    g.build();
    // printf("%d\n", g.inquire());
    if (m) {
        ModInt sum = 0;
        const ModInt comb = binom(n+m, n+1);
        for (int i = 1; i < n; i++) {
            g.update(pos[i]);
            sum += g.inquire();
            // printf("upd=%d, %d\n", pos[i], g.inquire());
        }
        ans += sum * comb;
    }
    /* for (int v = 0; v <= n; v++)
        b[v] = f.sum(v); // > v count
    for (int i = 1; i <= n; i++) {
        g.add(a[i], 1);
        f.add(a[i], -1);
        for (int v = 0; v <= n; v++) {
            printf("%d%c", i - g.sum(v) + f.sum(v), v==n?'\n':' ');
            b[v] = std::min(b[v], i - g.sum(v) + f.sum(v));
        }
    }
    if (m) ans += binom(n+m, n+1) * std::accumulate(b, b+n+1, ModInt());
    printf("%d\n", int(std::accumulate(b, b+n+1, ModInt()))); */
    printf("%d\n", int(ans));
    /* for (int i = 1; i <= p; i++) {
        int frontCnt = 0, backCnt = 0;
        for (int j = 1; j < i; j++)
            if (a[j] > a[p]) ++frontCnt;
        for (int j = i; j < p; j++)
            if (a[j] < a[p]) ++backCnt;
        printf("Between pos %d and %d, frontGt=%d, backLt=%d, tot=%d\n", i-1, i, frontCnt, backCnt, frontCnt + backCnt);
    } */
    return 0;
}