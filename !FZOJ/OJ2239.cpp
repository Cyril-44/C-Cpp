#include <stdio.h>
#include <string.h>
#include <queue>
const int N = 1005, M = 10000 * 2 + 1000 * 2 + 5;
struct Edge{
	int to, nxt, w;
} e[M];
int head[N], htop, mx, n;
inline void addedg(const int &fr, const int &to, const int &limit) {
	e[++htop] = (Edge) {to, head[fr], limit};
	head[fr] = htop;
}
bool vis[N];
int cnt[N];
int d[N];
inline bool spfa(int u = 0) {
	std::queue<int> q;
	q.push(u);
	memset(d, 0x3f, sizeof d);
	memset(vis, 0, sizeof vis);
	memset(cnt, 0, sizeof cnt);
	d[u] = 0, cnt[u] = 1;
	while (!q.empty()) {
		u = q.front(), q.pop(), vis[u] = false;
		for (int i = head[u], v; i; i = e[i].nxt) {
			if (d[v = e[i].to] > d[u] + e[i].w) {
				d[v] = d[u] + e[i].w;
				if (!vis[v]) {
					if (cnt[v] > n) return false;
					vis[v] = true;
					++cnt[v];
					q.push(v);
				}
			}
		}
	}
	return true;
}
int main() {
	int ml, md, a, b, d;
	scanf("%d%d%d", &n, &ml, &md);
	while (ml--) { // xa - xb >= d
		scanf("%d%d%d", &a, &b, &d);
		addedg(a, b, d);
	}
	while (md--) { // xa - xb <= d
		scanf("%d%d%d", &a, &b, &d);
		addedg(b, a, -d);
	}
	for (int i = 1; i < n; i++) {
		addedg(i+1, i, 0);
		addedg(0, i, 0);
	}
	if (!spfa()) puts("-1");
	else if (spfa(1) && ::d[n] == 0x3f3f3f3f) puts("-2");
	else printf("%d\n", ::d[n]);
	return 0;
}