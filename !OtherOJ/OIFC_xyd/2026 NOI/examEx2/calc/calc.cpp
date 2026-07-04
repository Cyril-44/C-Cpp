#include <cstdio>
#include <algorithm>
#include <cstring>
#include <vector>
constexpr int N = 5005, MOD = (int)1e9 + 7;
struct Mint {
    Mint(int v=0) : val(v) {}
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint a, Mint b) { return a += b; }
    friend Mint operator-(Mint a, Mint b) { return a -= b; }
    friend Mint operator*(Mint a, Mint b) { return a *= b; }
private: int val;
};
inline Mint qpow(Mint b, int n) {
    Mint res = 1;
    while (n) {
        if (n & 1) res *= b;
        b *= b, n >>= 1;
    }
    return res;
}
std::vector<int> g[N];
Mint f[N][N], tf[N]; // f[i][j] 
int sz[N];
void dfs(int u, int fa, bool flg) { // flg=0 表示父->子， flg=1 表示子->父
    sz[u] = 1, f[u][1] = 1;
    for (int v : g[u]) if (v != fa) {
        memset(tf, 0, sizeof tf);
        if (!flg) {
            for (int i = 1; i <= sz[u]; i++)
                for (int j = 1; j <= sz[v]; j++)
                    tf[i+j] += f[u][i] * f[v][j];
            memcpy()
        } else {
            for (int i = 1; i <= sz[u]; i++) {
                tf[i] += f[u][i] ( std::);
                for (int j = 1; j <= sz[v]; j++)
                    tf[i+j] -= f[u][i] * f[v][j];
            }
        }
        sz[u] += sz[v];
    }
}
int main() {
    int n, p, q;
    scanf("%d%d%d", &n, &p, &q);
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    if (p < q) {
        
    } else {

    }
    dfs(1, 0);
    return 0;
}