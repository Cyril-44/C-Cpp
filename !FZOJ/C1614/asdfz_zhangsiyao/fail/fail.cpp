#include <bits/stdc++.h>
#include <cstdint>
#ifdef CLANGD
constexpr int N = 25;
#else
constexpr int N = 200005;
#endif
#define L first
#define R second
using Pii = std::pair<int,int>;
Pii d[N], q[N];
struct Mint {
    constexpr static int MOD = (int)1e9 + 7;
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
};
int n, m;
namespace BF {
    constexpr int N = 1005;
    Mint sum[N][N];
    inline void work() {
        for (int i = 1; i <= n; i++) {
            std::set<Pii> st({{0,0}, {(int)1e9+1,(int)1e9+1}});
            int len = 0;
            for (int j = i; j <= n; j++) {
                auto it = --st.upper_bound({d[j].L, 0});
                int l = d[j].L, r = 0;
                if (it->R < d[j].L) ++it;
                else l = it->first;
                for (; it->L <= d[j].R; it = st.erase(it))
                    len -= it->R - it->L, r = it->R;
                r = std::max(r, d[j].R);
                st.emplace(l, r), len += r - l;
                sum[i][j] = sum[i][j-1] + len;
            }
        }
        for (int i = 1; i <= m; i++) {
            Mint ans = 0;
            for (int j = q[i].L; j <= q[i].R; j++)
                ans += sum[j][q[i].R];
            ans *= Mint((q[i].R - q[i].L + 1) * (q[i].R - q[i].L + 2) / 2).inv();
            printf("%d\n", ans);
        }
    }
}
int main() {
    freopen("fail.in", "r", stdin);
    freopen("fail.out", "w", stdout);
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d%d", &d[i].L, &d[i].R);
    for (int i = 1; i <= m; i++)
        scanf("%d%d", &q[i].L, &q[i].R);
    BF::work();
}