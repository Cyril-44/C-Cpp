#include <bits/stdc++.h>
#include <cmath>
#include <cstdint>
#ifdef CLANGD
constexpr int N = 25;
#else
constexpr int N = 200005;
#endif
#define L first
#define R second
using Pii = std::pair<int,int>;
Pii d[N];
std::pair<Pii, int> q[N];
struct Mint {
    constexpr static int MOD = (int)1e9 + 7;
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    Mint operator-() const { return val ? Mint(MOD - val) : 0; }
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
    template<typename T> explicit operator T() const { return static_cast<T>(val); }
private: int val;
};
int n, m;
Mint ans[N];
class BIT {
    Mint tr[N];
    void upd(int p, Mint x) { for (; p <= n; p += p & -p) tr[p] += x; }
    Mint sum(int p) const { Mint res = 0; for (; p > 0; p -= p & -p) res += tr[p]; return res; }
public:
    void rangeadd(int l, int r, Mint x) { if (int(x) && l <= r) upd(l, x), upd(r+1, -x); }
    Mint operator[](int p) const { return sum(p); }
} flr, fl, fr, fc;
inline void update(int l, int r, int len, Mint xlr, Mint xl, Mint xr, Mint xc) {
    // fprintf(stderr, "[%d,%d] len=%d lr+=%d, l+=%d, r+=%d, c+=%d\n", l, r, len, xlr, xl, xr, xc);
    flr.rangeadd(l, r, xlr * len);
    fl.rangeadd(l, r, xl * len);
    fr.rangeadd(l, r, xr * len);
    fc.rangeadd(l, r, xc * len);
}
int main() {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d%d", &d[i].L, &d[i].R);
    for (int j = 1; j <= m; j++) {
        scanf("%d%d", &q[j].first.L, &q[j].first.R);
        q[j].second = j;
    }
    std::sort(q+1, q+1+m, [](const auto &x, const auto &y) { return x.first.R < y.first.R; });
    std::map<std::pair<int,int>, int> st;
    st[{0,1}] = st[{1, (int)1e9}] = st[{(int)1e9,(int)1e9+1}] = 0;
    for (int i = 1, j = 1; j <= m; j++) {
        if (q[j].first.R > n) continue;
        for (; i <= q[j].first.R; i++) {
            auto itl = --st.lower_bound({d[i].L, 0});
            if (itl->first.R > d[i].L) {
                auto [rg, p] = *itl; itl = st.erase(itl);
                st[{rg.L, d[i].L}] = p;
                st[{d[i].L, rg.R}] = p;
                --itl;
            }
            else ++itl;
            auto itr = --st.upper_bound({d[i].R, 0});
            if (itr->first.R > d[i].R) {
                bool syncitl = itl == itr;
                auto [rg, p] = *itr; itr = st.erase(itr);
                st[{rg.L, d[i].R}] = p;
                st[{d[i].R, rg.R}] = p;
                --itr;
                if (syncitl) itl = std::prev(itr);
            }
            else ++itr;
            for (auto it = itl; it != itr; it = st.erase(it)) {
                auto [rg, p] = *it;
                update(1, p, rg.R - rg.L, 0, 0, i - p, Mint(i) * (p - Mint(i)));
                update(p+1, i, rg.R - rg.L, -Mint(1), i, i, -Mint(i)*i);
            }
            st[d[i]] = i;
        }
        // fprintf(stderr, "Until %d: ", q[j].first.R);
        // for (const auto &[x, y] : st) fprintf(stderr, "[%d,%d]=%d ", x.L, x.R, y);
        // fprintf(stderr, "\n");
        int l = q[j].first.L - 1, r = q[j].first.R + 1;
        // fprintf(stderr, "cnt[%d] = %d * %d*%d + %d * %d + %d * %d + %d\n", q[j].second, flr[l+1], l, r, fl[l+1], l, fr[l+1], r, fc[l+1]);
        ans[q[j].second] = (flr[l+1]*l*r + fl[l+1]*l + fr[l+1]*r + fc[l+1]) * Mint((r-l)*(r-l-1ull)/2 % Mint::MOD).inv();
    }
    for (int i = 1; i <= m; i++)
        printf("%d\n", ans[i]);
}