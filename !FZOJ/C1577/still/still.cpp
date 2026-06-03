#include <cstdio>
#include <vector>
#include <numeric>
struct ModInt {
    constexpr static int mod() { return 998244353; }
    ModInt& operator+=(ModInt x) { if ((val += x.val) >= mod()) val -= mod(); return *this; }
    ModInt& operator-=(ModInt x) { if ((val -= x.val) < 0) val += mod(); return *this; }
    ModInt& operator*=(ModInt x) { val = 1ull*val*x.val%mod(); return *this; }
    ModInt operator+(ModInt x) const { return ModInt(*this) += x; }
    ModInt operator-(ModInt x) const { return ModInt(*this) -= x; }
    ModInt operator*(ModInt x) const { return ModInt(*this) *= x; }
    ModInt(int x = 0) : val(x) {}
    ModInt(long long x) : val(x%mod()) { *this += mod(); }
    template<class T> operator T() const { return static_cast<T>(val); }
private: int val;
};
int n, m, k;
struct DSU {
    DSU(int n) : fa(n+1) { std::iota(fa.begin(), fa.end(), 0); }
    int find(int u) { return u ^ fa[u] ? fa[u] = find(fa[u]) : u; }
    bool unite(int u, int v) {
        u = find(u), v = find(v);
        if (u == v) return false;
        fa[u] = v; return true;
    }
    std::vector<int> fa;
};
namespace BF1 {
    constexpr int N = 7;
    int h[N], fa[N];
    int ans = 0;
    inline void check() {
        // for (int i = 1; i <= n; i++) printf("%d ", h[i]);
        // for (int i = 2; i <= n; i++) printf("%d ", fa[i]);
        DSU dsu(n);
        for (int i = 2; i <= n; i++) {
            if (!dsu.unite(i, fa[i]) || h[std::max(i, fa[i])] <= h[std::min(i, fa[i])]) {
                // puts("Rejected!");
                return;
            }
        }
        // puts("Pass");
        ++ans;
    }
    void dfs(int u) {
        if (u > n) return check();
        for (h[u] = 1; h[u] <= m; h[u]++)
            for (fa[u] = 1; fa[u] <= n; fa[u]++)
                dfs(u+1);
    }
    inline void work() {
        ans = 0;
        for (h[1] = 1; h[1] <= m; h[1]++) dfs(2);
        printf("%d\n", ans);
    }
};
int main() {
    scanf("%d%d%d", &n, &m, &k);
    if (n <= 6 && m <= 3 && k == 0) return BF1::work(), 0;
    if (k == m) return putchar(n==1?'1':'0'), 0;
    ModInt ans = 1;
    for (int i = 1; i < n; i++) ans *= i;
    printf("%d\n", int(ans));
    return 0;
}