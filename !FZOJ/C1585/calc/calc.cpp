#include <bits/stdc++.h>
constexpr int N = 100001, K = 101, MOD = (int)1e9 + 7;
#pragma GCC optimize(3)
#pragma GCC optimize("unroll-loops", "no-stack-protector", "inline")
struct FastI {
    [[gnu::always_inline]] inline char get() {
        if (p1 == p2) p2 = (*buf='\0', p1=buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++;
    }
    [[gnu::always_inline]] inline void operator()(int &x) {
        char ch = get();
        while (ch < '0' || ch > '9') ch = get();
        for (x = 0; ch >= '0' && ch <= '9'; ch = get())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
private:
    char buf[1 << 20], *p1{}, *p2{};
} fin; 

struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint a, Mint b) { return a += b; }
    friend Mint operator-(Mint a, Mint b) { return a -= b; }
    friend Mint operator*(Mint a, Mint b) { return a *= b; }
    Mint(int o = 0) : val(o) {}
private: int val;
} f[N][K], C[K][K], tmp[K];
int fa[N], sz[N];
#define For(i, s, t) for (int i = (s); i <= (t); i++)
#define roF(i, s, t) for (int i = (s); i >= (t); i--)
/*
  sum(连通块 乘积 ^ k)
= sum(所有连通块中选 k 次点的方案数 乘积)

令 f[u][i] 表示以 u 为根连通块，选了 k 个点的方案数乘积
*/
int main() {
    int n, k;
    fin(n), fin(k);
    For(i, 2, n) fin(fa[i]);
    For(i, 0, k) {
        C[i][0] = C[i][i] = 1;
        For(j, 1, i-1)
            C[i][j] = C[i-1][j-1] + C[i-1][j];
    }

    For(i, 1, n) f[i][0] = 1, sz[i] = 1;
    roF(v, n, 2) {
        int u = fa[v];
        roF(i, k, 0) roF(j, i-1, 0)
            f[v][i] += f[v][j] * C[i][j];
        For(j, 0, k) tmp[j] = f[u][j] * f[v][k];
        For(j, 0, k)
            For(i, 0, k-j)
                tmp[i+j] += f[u][i] * f[v][j] * C[i+j][j];
        sz[u] += sz[v];
        memcpy(f[u], tmp, sizeof tmp);
    }
    roF(i, k, 0) roF(j, i-1, 0)
        f[1][i] += f[1][j] * C[i][j];
    printf("%d\n", f[1][k]);
    return 0;
}