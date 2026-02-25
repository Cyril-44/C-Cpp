#include <stdio.h>
#include <vector>
#include <queue>
using LL = long long;
struct FastIS {
	char buf[1 << 20], *p1, *p2;
	FastIS() : p1(nullptr), p2(nullptr) {}
	__attribute__((always_inline)) inline int get() {
		return (p1 == p2) && (p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin), p1 == p2) ? EOF : *p1++;
	}
} is;
template<typename T, typename = typename std::enable_if<std::is_integral<T>::value, void>::type>
inline void in(T& x) {
	int ch = is.get();
	while (~ch && (ch < '0' || ch > '9')) ch = is.get();
	for (x = 0; ch >= '0' && ch <= '9'; ch = is.get())
		x = (x << 3) + (x << 1) + (ch ^ '0');
}
constexpr int N = (int)1e6 + 5, MOD = (int)1e9 + 7;
int dep[N], pr[N], cntlf[N];
bool islf[N];
std::vector<int> g[N], vec[N];
inline int qpow(int b, int n) {
    int res = 1;
    while (n) {
        if (n & 1) res = 1ull * res * b % MOD;
        b = 1ull * b * b % MOD, n >>= 1;
    }
    return res;
}
void dfs1(int u, int fa, int prob) {
    if (g[u].size() == 1) islf[u] = 1;
    dep[u] = dep[fa] + 1;
    pr[u] = prob;
    for (int v : g[u]) {
        if (v == fa) continue;
        dfs1(v, u, 1ull * prob * qpow(g[u].size() - (bool)fa, MOD - 2) % MOD);
    }
}
int dfs(int u) {
    std::vector<int> vv;
    std::queue<int> q({u});
    int curdep = 0, ans = 0;
    bool flg = true;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (dep[u] > curdep) {
            if (cntlf[curdep] > 1)
                { flg = false; break; }
            cntlf[curdep = dep[u]] = 0;
            vec[curdep].clear();
        }
        vec[curdep].push_back(u);
        cntlf[curdep] += islf[u];
        for (int y : g[u])
            if (dep[y] > dep[u])
                q.push(y);
    }
    if (!flg) {
        int iu = qpow(pr[u], MOD - 2);
        for (int v : vec[curdep]) {
            if (islf[v]) continue;
            vv.push_back(v);
            ans = (ans + 1ull * pr[v] * iu) % MOD;
        }
        for (int v : vv) ans = (ans + 1ull * dfs(v) * pr[v] % MOD * iu % MOD) % MOD;
    }
    return ans;
}
int main() {
    int n; in(n);
    for (int i = 1, x, y; i < n; i++) {
        in(x), in(y);
        g[x].push_back(y);
        g[y].push_back(x);
    }
    dfs1(1, 0, 1);
    printf("%d\n", dfs(1));
    return 0;
}