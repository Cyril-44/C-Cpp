#include <cstdio>
constexpr int N = 
#ifdef CLANGD
64
#else
600004
#endif
, K = 60, _N = N * K, MOD = 998244353;
unsigned long long m, a[N];
int sz[_N], ch[_N][2];
int top;
struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint a, Mint b) { return a += b; }
    friend Mint operator-(Mint a, Mint b) { return a -= b; }
    friend Mint operator*(Mint a, Mint b) { return a *= b; }
    Mint(int o = 0) : val(o) {}
private: int val;
} pw2[N];
Mint dfs(int cur, int u, int v) {
    if (!u || !v) return sz[u|v]+1;
    if (cur < 0) return Mint(sz[u]+1) * (sz[v]+1);
    if (m >> cur & 1) { // cur >= 1
        return dfs(cur-1, ch[u][0], ch[v][1]) + dfs(cur-1, ch[u][1], ch[v][0]) - 1;
    } else {
        return dfs(cur-1, ch[u][0], ch[v][0]) + dfs(cur-1, ch[u][1], ch[v][1]) - 1
            + Mint(sz[ch[u][0]]) * sz[ch[v][1]] + Mint(sz[ch[u][1]]) * sz[ch[v][0]];
    }
}
Mint calc(int cur, int u) {
    if (!u) return 1;
    if (cur < 0) return pw2[sz[u]];
    if (m >> cur & 1) { // cur >= 1
        return dfs(cur-1, ch[u][0], ch[u][1]);
    } else { // cur >= 0
        return calc(cur-1, ch[u][0]) * calc(cur-1, ch[u][1]);
    }
}
int main() {
    int tid, n;
    scanf("%d%d%llu", &tid, &n, &m);
    pw2[0] = 1; for (int i = 1; i <= n; i++) pw2[i] = pw2[i-1] + pw2[i-1];
    top = 1;
    for (int i = 1; i <= n; i++) {
        scanf("%llu", &a[i]);
        int u = 1;
        for (int k = K-1; k >= 0; k--) {
            bool f = a[i] >> k & 1;
            if (!ch[u][f]) ch[u][f] = ++top;
            ++sz[u = ch[u][f]];
        }
    }
    printf("%d\n", calc(K-1, 1));
    return 0;
}