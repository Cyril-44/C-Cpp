#include <bits/stdc++.h>
#ifdef CLANGD
constexpr int N = 15;
#else
constexpr int N = 1000005;
#endif
int n;
struct BIT {
    int tr[N];
    void upd(int p, int x) { for (; p <= n; p += p & -p) tr[p] += x; }
    int sum(int p) { int res = 0; for (; p > 0; p -= p & -p) res += tr[p]; return res; }
} f;
struct Node {
    int p, q;
    bool operator<(const Node &o) const { return p < o.p; }
} a[N], tmp[N];
struct Mint {
    constexpr static int MOD = 998244353;
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    Mint pow(int n) const {
        Mint res = 1, b = *this;
        while (n) {
            if (n & 1) res *= b;
            b *= b, n >>= 1;
        }
        return res;
    }
    Mint inv() const { return pow(MOD - 2); }
    Mint(int v=0) : val(v) {}
private: int val;
} ans;
int cnt[N];
Mint i2, iuk;
void cdq(int l, int r) {
    if (l == r) return;
    int mid = l + r >> 1;
    cdq(l, mid), cdq(mid+1, r);
    Mint ukval; int luk = 0;
    int i = l, j = mid+1;
    for (; j <= r; j++) {
        for (; i <= mid && a[i].p < a[j].p; i++) {
            if (~a[i].q) f.upd(a[i].q, 1), ukval += 1 - cnt[a[i].q] * iuk;
            else ukval += i2, ++luk;
        }
        if (~a[j].q) ans += f.sum(a[j].q) + Mint(luk) * cnt[a[j].q] * iuk;
        else ans += ukval;
        // fprintf(stderr, "[%d,%d) --> %d: ans=%d\n", l, i, j, ans);
    }
    while (i > l) if (~a[--i].q) f.upd(a[i].q, -1);
    i = l, j = mid+1; int idx = l;
    while (i <= mid && j <= r) {
        if (a[i].p < a[j].p) tmp[idx++] = a[i++];
        else tmp[idx++] = a[j++];
    }
    while (i <= mid) tmp[idx++] = a[i++];
    while (j <= r) tmp[idx++] = a[j++];
    memcpy(a+l, tmp+l, sizeof(Node) * (r-l+1));
}
int main() {
    freopen("per.in", "r", stdin);
    freopen("per.out", "w", stdout);
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) cnt[i] = 1, scanf("%d", &a[i].p);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i].q);
        if (~a[i].q) cnt[a[i].q] = 0;
    }
    for (int i = 1; i <= n; i++) cnt[i] += cnt[i-1];
    i2 = Mint(2).inv(), iuk = Mint(cnt[n]).inv();
    cdq(1, n);
    printf("%d\n", ans);
}