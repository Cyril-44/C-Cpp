#include <bits/stdc++.h>
constexpr int N = 200005, MOD = (int)1e9 + 7;
#ifdef CLANGD
#define N 25
#endif
std::vector<int> g[N];
struct Mint {
    Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(const Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(const Mint o) { val = 1ull * val * o.val % MOD; return *this;  }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator-(Mint x, const Mint y) { return x -= y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    Mint pow(int n) const {
        Mint b = *this, res = 1;
        for (; n; n >>= 1, b *= b) if (n&1) res*=b;
        return res;
    }
    Mint(int v=0) : val(v) {}
private: int val;
} _f[N], *f[N], _t[N], *tag[N];
int sz[N], len[N], lson[N], dfn[N];
void dfs1(int u, int fa) {
    sz[u] = 1;
    for (int v : g[u]) if (v != fa) {
        dfs1(v, u);
        sz[u] += sz[v];
        if (len[v] >= len[u]) {
            len[u] = len[v] + 1;
            lson[u] = v;
        }
    }
}
void dfs2(int u, int fa) {
    dfn[u] = ++dfn[0];
    f[u] = _f + dfn[u];
    tag[u] = _t + dfn[u];
    f[u][0] = 1, tag[u][0] = 1;
    if (!lson[u]) return;
    dfs2(lson[u], u);
    for (int v : g[u]) if (v != fa && v != lson[u]) {
        dfs2(v, u);
        for (int i = 1; i <= len[v]; i++) {
            tag[u][i+1] *= tag[u][i];
            f[u][i] *= tag[u][i];
            tag[u][i+1] = 1;
            f[u][i] *= f[v][i];
        }
        tag[u][len[v] + 1] *= f[v][len[v]];
    }
}
int main() {
    freopen("game.in", "r", stdin);
    freopen("game.out", "w", stdout);
    int n;
    scanf("%d", &n);
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs1(1, 0), dfs2(1, 0);
    auto vs = g[1];
    Mint ans = 1, perma = 1;
    fprintf(stderr, "%zu\n", vs.size());
    for (int v : vs)
        for (int i = 0; i <= len[v]; i++)
            fprintf(stderr, "%d%c", f[v][i], " \n"[i==len[v]]);
    for (int i = 0; i < len[1]; i++) {
        std::vector<int> nvs;
        std::vector<Mint> vals;
        Mint prod = 1;
        for (int v : vs) {
            auto val = f[v][i];
            if (i) f[v][i] += f[v][i-1];
            val *= f[v][i].pow(MOD - 2);
            vals.push_back(val);
            if (len[v] < i) prod *= f[v][i], nvs.push_back(v);
            else perma *= f[v][i];
        }
        Mint presum = 0, sum = 0;
        for (Mint val : vals) {
            sum += presum * val;
            presum += val;
        }
        prod *= perma, vs = nvs;
        ans += prod * sum;
    }
    printf("%d\n", ans);
}