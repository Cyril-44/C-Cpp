#include <stdio.h>
const int N = 300005, N2 = 19;
inline void in(int &x) {
	char ch = getchar();
	while (ch < '0' || ch > '9')
		ch = getchar();
	for (x = 0; ch >= '0' && ch <= '9'; ch = getchar())
		x = (x << 3) + (x << 1) + (ch ^ '0');
}
struct Edge {
	int to, nxt, w;
} e[N << 1];
int head[N], f[N], d[N], fa[N][20];
inline void addedg(const int &fr, const int &to) {
	e[++head[0]] = {to, head[fr]};
	head[fr] = head[0];
}
inline void initFa(const int &u, const int &p) {
	fa[u][0] = p;
	for (int i = head[u]; i; i = e[i].nxt)
		if (e[i].to ^ p) {
			d[e[i].to] = d[u] + 1;
			initFa(e[i].to, u);
		}
}
inline void buildFa(const int &n) {
	for (int k = 1; k <= N2; k++)
		for (int i = 1; i <= n; i++)
			fa[i][k] = fa[fa[i][k-1]][k-1];
}
inline int lca(int x, int y) {
	register int jmp = d[x] - d[y];
	if (jmp < 0) x ^= y ^= x ^= y, jmp = -jmp;
	for (int i = N2; i >= 0; i--)
		if (1<<i & jmp) x = fa[x][i];
	if (x == y) return x;
	for (int i = N2; i >= 0; i--)
		if (fa[x][i] ^ fa[y][i])
			x = fa[x][i], y = fa[y][i];
	return fa[x][0];
}
inline void dfs(const int &u, const int &p) {
	for (int i = head[u], v; i; i = e[i].nxt) {
		v = e[i].to;
		if (v ^ p) {
			dfs(v, u);
			f[u] += f[v];
		}
	}
}
int main() {
	int n, m, x, y;
	scanf("%d %d", &n, &m);
	for (int i = 1; i < n; i++) {
		in(x), in(y);
		addedg(x, y);
		addedg(y, x);
	}
	initFa(1, 0);
	buildFa(n);
	for (int i = 0; i < m; i++) {
		in(x), in(y);
		++f[x], ++f[y];
		f[lca(x, y)] -= 2;
	}
	dfs(1, 0);
	long long ans = 0;
	for (int i = 2; i <= n; i++)
		switch (f[i]) {
			case 0: ans += m; break;
			case 1: ++ans;    break;
		}
	printf("%lld\n", ans);
	return 0;
}