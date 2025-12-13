#include <stdio.h>
#include <string.h>
const int N = 20;
int d[N][N], deg[N], n, res[1 << 16];
inline void umin(int &x, const int &y) {
	x = x < y ? x : y;
}
inline int dfs(const int &status) {
	if (res[status] ^ -1) return res[status];
	if (!status) return res[status] = 0;
	res[status] = 0x3fffffff;
	for (int i = 1; i <= n; i++)
	if (1<<i & status)
		for (int j = 1; j <= n; j++)
		if ((i^j) && (1<<j & status))
			umin(res[status], d[i][j] + dfs(status ^ (1<<i | 1<<j)));
	return res[status];
}
int main() {
	int m, u, v, w, ans = 0;
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			d[i][j] = (i^j) ? 0x3fffffff : 0;
	while (m--) {
		scanf("%d%d%d", &u, &v, &w);
		umin(d[u][v], w);
		umin(d[v][u], w);
		++deg[u], ++deg[v];
		ans += w;
	}
	for (int k = 1; k <= n; k++)
		for (int i = 1; i <= n; i++)
			for (int j = 1; j <= n; j++)
				if ((k^i) && (i^j) && (j^k))
					umin(d[i][j], d[i][k] + d[k][j]);
	int status = 0;
	memset(res, -1, sizeof res);
	for (int i = 1; i <= n; i++)
		if (deg[i] & 1) status |= 1 << i;
	printf("%d\n", ans + dfs(status));
	return 0;
}