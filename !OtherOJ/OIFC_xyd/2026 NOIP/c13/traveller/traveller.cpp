#include <bits/stdc++.h>
constexpr int N = 5005, MOD = (int)1e9 + 7;
std::pair<int,int> edg[N];
std::vector<int> g[N];
struct Mint {
    Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(const Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(const Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator-(Mint x, const Mint y) { return x -= y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    Mint operator-() const { return val ? Mint(MOD - val) : Mint(); }
    Mint pow(int n) const {
        Mint res = 1, b = *this;
        for (; n; n >>= 1, b *= b)
            if (n & 1) res *= b;
        return res;
    }
    Mint(int v=0) : val(v) {}
private: int val;
} fact[N];
int main() {
    freopen("travel.in", "r", stdin);
    freopen("travel.out", "w", stdout);
    int n;
    scanf("%d", &n);
    fact[0] = 1;
    for (int i = 1; i <= n; i++) fact[i] = fact[i-1] * i;
    bool isChain = true, isStar = true;
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        edg[i] = {u, v};
        g[u].push_back(v);
        g[v].push_back(u);
        if (!(u == i && v == i+1)) isChain = false;
        if (!(u == 1 && v == i+1)) isStar = false;
    }
    if (isChain) {
        Mint ans = n-1;
        for (int i = 1; i < n; i++)
            ans += (fact[n-i] - fact[n-i-1]) * i;
        printf("%d\n", ans * fact[n].pow(MOD-2));
    } else if (isStar) {
        Mint ans = 0;
        for (int i = 1; i < n; i++)
            ans += fact[n-1] * (2*i-1);
        printf("%d\n", ans * fact[n].pow(MOD-2));
    } else {
        constexpr int N = 25;
        Mint ans = 0;
        int dep[N][N]{}; bool vis[N];
        for (int s = 1; s <= n; s++) {
            memset(vis, 0, sizeof vis);
            std::queue<int> q;
            q.push(s); vis[s] = true;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (int v : g[u]) if (!vis[v])
                    vis[v] = true, dep[s][v] = dep[s][u] + 1, q.push(v);
            }
        }
        int p[N];
        std::iota(p+1, p+1+n, 1);
        do {
            memset(vis, 0, sizeof vis);
            vis[1] = 1;
            int now = 1, len = 0;
            for (int i = 1; i < n; i++) {
                if (vis[edg[p[i]].first]) {
                    len += dep[now][edg[p[i]].second];
                    now = edg[p[i]].second;
                } else if (vis[edg[p[i]].second]) {
                    len += dep[now][edg[p[i]].first];
                    now = edg[p[i]].first;
                }
                else break;
                vis[now] = true;
            }
            ans += len;
        } while (std::next_permutation(p+1, p+1+n));
        printf("%d\n", ans * fact[n].pow(MOD-2));
    }
}