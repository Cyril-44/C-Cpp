#include <stdio.h>
#include <string.h>
#include <algorithm>
constexpr int N = 100005, D = 17;
struct Edge { int to, nxt; } e[N << 1]; int head[N];
inline void addedg(int fr, int to) {
	e[++head[0]] = Edge {to, head[fr]};
	head[fr] = head[0];
}
int fa[N][D+1], dep[N], c[N];
void init(int u, int pre) {
	dep[u] = dep[pre] + 1, fa[u][0] = pre;
	for (int i = 1; i <= D; i++)
		fa[u][i] = fa[fa[u][i-1]][i-1];
	for (int i = head[u]; i; i = e[i].nxt)
		if (e[i].to ^ pre) init(e[i].to, u);
}
inline int lca(int u, int v) {
	if (dep[u] < dep[v]) u^=v^=u^=v;
	for (int x = dep[u] - dep[v], i = D; i >= 0; i--)
		if (x >> i & 1) u = fa[u][i];
	if (u == v) return u;
	for (int i = D; i >= 0; i--)
		if (fa[u][i] ^ fa[v][i])
			u = fa[u][i], v = fa[v][i];
	return fa[u][0];
}
inline int dist(int u, int v) {
	if (~u && ~v) {
		int com = lca(u, v);
		return dep[u] + dep[v] - (dep[com] << 1);
	}
	return -1;
}
struct Diam {
	int u, v, d;
	inline Diam() : u(-1), v(-1), d(-1) {}
	inline Diam(int _u, int _v, int _d) : u(_u), v(_v), d(_d) {}
} diam[N];
inline void updDiam(Diam& d, int p) {
    if (d.u == -1) {
        d.u = d.v = p;
        d.d = 0;
        return;
    }
    if (d.v == -1) {
        d.v = p;
        d.d = dist(d.u, d.v);
        return;
    }
    int pdu = dist(p, d.u), pdv = dist(p, d.v);
    if (pdu > pdv) {
        if (pdu > d.d) d = Diam(d.u, p, pdu);
    } else {
        if (pdv > d.d) d = Diam(p, d.v, pdv);
    }
}

int ans;
inline void merge(Diam& dest, const Diam& src) {
    if (src.u == -1) return;        // src 没有效点
    if (dest.u == -1) { dest = src; return; } // dest 为空则直接赋值
	int adc = dist(dest.u, src.u),
		add = dist(dest.u, src.v),
		bdc = dist(dest.v, src.u),
		bdd = dist(dest.v, src.v);
	int mx = std::max({adc, add, bdc, bdd});
	ans = std::max(ans, mx);
	mx = std::max({mx, dest.d, src.d});
	if (adc == mx) dest = Diam(dest.u, src.u, adc);
	else if (add == mx) dest = Diam(dest.u, src.v, add);
	else if (bdc == mx) dest = Diam(dest.v, src.u, bdc);
	else if (bdd == mx) dest = Diam(dest.v, src.v, bdd);
	else if (src.d == mx) dest = src;
}
int main() {
	int n, m, q;
	scanf("%d%d%d", &n, &m, &q);
	for (int i = 1; i <= n; i++)
		scanf("%d", &c[i]);
	for (int i = 1, u, v; i < n; i++) {
		scanf("%d%d", &u, &v);
		addedg(u, v);
		addedg(v, u);
	}
	init(1, 1);
	for (int i = 1; i <= n; i++)
		updDiam(diam[c[i]], i);
	while (q--) {
		int k;
		scanf("%d", &k);
		int di;
		scanf("%d", &di);
		Diam now = diam[di];
		ans = -1;
		for (int i = 1; i < k; i++) {
			scanf("%d", &di);
			merge(now, diam[di]);
		}
		printf("%d\n", ans >= 0 ? ans + 1 : -1);
	}
	return 0;
}
