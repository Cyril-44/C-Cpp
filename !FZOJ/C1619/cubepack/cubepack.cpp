#include <bits/stdc++.h>
constexpr int N = 105, M = 50005, MOD = (int)1e7 + 7;
struct Mint {
    Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(const Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(const Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator-(Mint x, const Mint y) { return x -= y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    Mint(int v=0) : val(v) {}
private: int val;
} f[M];
std::vector<int> e[N];
int v[N], c[N];
bool flg[N];
void dfs(int u) {
    flg[u] = true;
    for (int v : e[u]) dfs(v);
}
int main() {
    int n, w, k;
    scanf("%d%d%d", &n, &w, &k);
    f[0] = 1;
    for (int i = 1, h; i <= n; i++) {
        scanf("%d%d%d", &v[i], &c[i], &h);
        e[h].push_back(i);
    }
    dfs(k);
    for (int i = 1; i <= n; i++) if (!flg[i]) {
        int c = std::min(::c[i], w / v[i]);
        if (!c) continue;
        for (int j = v[i]; j <= w; j++)
            f[j] += f[j - v[i]];
        for (int j = w; j >= v[i] * (c+1); j--)
            f[j] -= f[j - v[i] * (c+1)];

    }
    printf("%d\n", f[w]);
}