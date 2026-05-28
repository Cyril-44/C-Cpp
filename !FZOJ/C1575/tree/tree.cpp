#include <cstdio>
#include <vector>
#include <algorithm>
#include <bitset>
constexpr int N = 100005, MOD = (int)1e9 + 9;
std::vector<std::pair<int,int>> g[N];
int n, m, k;
struct ModInt {
    ModInt(int v=0) : val(v) {}
    ModInt operator-() const { return ModInt(!val ? 0 : MOD-val); }
    ModInt& operator+=(ModInt x) { if ((val += x.val) >= MOD) val -= MOD; return *this; }
    ModInt& operator-=(ModInt x) { if ((val -= x.val) < 0) val += MOD; return *this; }
    ModInt& operator*=(ModInt x) { val = 1ull*val*x.val % MOD; return *this; }
    ModInt operator+(ModInt x) const { return ModInt(*this) += x; }
    ModInt operator-(ModInt x) const { return ModInt(*this) -= x; }
    ModInt operator*(ModInt x) const { return ModInt(*this) *= x; }
    template<class T> constexpr explicit operator T() const { return static_cast<T>(val); }
    private: int val;
};
/* namespace BruteForce {
constexpr int N = 10;
std::bitset<N> curst, all[N];
ModInt C[N][N];
void getall(int u, int pre, int dep) {
    if (dep > k) return;
    curst.set(u);
    for (auto [v, w] : g[u]) if (v != pre) getall(v, u, dep + w);
}
inline void work() {
    for (int i = 1; i <= n; i++)
        curst.reset(), getall(i, 0, 0), all[i] = curst;
    for (int i = 1; i <= n; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++) C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    ModInt ans = 0;
    std::vector<unsigned> maskall[N];
    for (unsigned mask = 0; !(mask>>n); mask++)
        maskall[__builtin_popcount(mask)].push_back(mask << 1);
    for (int i = 1; i <= n; i++) {
        for (unsigned mask : maskall[i]) {
            std::bitset<N> st; st.set();
            for (int j = 1; j <= n; j++)
                if (mask >> j & 1) st &= all[j];
            int sz = st.count();
            if (i & 1) ans += C[sz][m];
            else ans -= C[sz][m];
        }
    }
    printf("%d\n", int(ans));
}
} */
int main() {
    int tid, T;
    scanf("%d%d", &tid, &T);
    while (T--) {
        scanf("%d%d%d", &n, &m, &k);
        for (int i = 1; i <= n; i++) g[i].clear();
        for (int i = 1, u, v, w; i < n; i++) {
            scanf("%d%d%d", &u, &v, &w);
            g[u].emplace_back(v, w);
            g[v].emplace_back(u, w);
        }
        
    }
    return 0;
}