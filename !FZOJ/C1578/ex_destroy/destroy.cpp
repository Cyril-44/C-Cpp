#include <cstdio>
#include <vector>
constexpr int N = 200005, M = 101, MOD = (int)1e9+7;
std::vector<int> g[N];
int nxt[N], dep[N];
int a[N], smx[N];
int dfs(int u, int fa) {
    int mx = u;
    for (int v : g[u]) if (v != fa) {
        dep[v] = dep[u] + 1; int mxv = dfs(v, u);
        if (dep[mxv] > dep[mx]) mx = mxv, nxt[u] = v;
    }
    return mx;
}
struct ModInt {
    ModInt& operator+=(const ModInt x) { if ((val += x.val) >= MOD) val -= MOD; return *this; }
    ModInt& operator-=(const ModInt x) { if ((val -= x.val) < 0) val += MOD; return *this; }
    ModInt& operator*=(const ModInt x) { val = 1ull*val*x.val%MOD; return *this; }
    friend ModInt operator+(ModInt x, const ModInt y) { return x += y; }
    friend ModInt operator-(ModInt x, const ModInt y) { return x -= y; }
    friend ModInt operator*(ModInt x, const ModInt y) { return x *= y; }
    ModInt(int v=0) : val(v) {}
    template<class T> explicit operator T() const { return static_cast<T>(val); }
private: int val;
} f[M][M][M];

int main() {
    int n;
    scanf("%d", &n);
    for (int u, v, i = 1; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dep[1] = 1;
    int m = dep[dfs(1, 0)];
    for (int i = 1; i < m; i++)
        a[i+1] = nxt[a[i]];
    for (int i = 1; i <= m; i++)
        for (int v : g[a[i]])
            
    return 0;
}
