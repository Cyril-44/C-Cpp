#include <cstdio>
#include <algorithm>
#include <cstring>
#include <queue>
const int N = 1005;
struct Edge {
	int to, nxt, w;
} e[N << 2];
int head[N];
inline void addedg(const int &fr, const int &to, const int &w) {
	e[++head[0]] = Edge{to, head[fr], w};
	head[fr] = head[0];
}
inline bool spfa(const int &s, const int &n) {
	std::queue<int> q;
	static bool vis[N];
	static int visc[N];
	static long long d[N];
	memset(vis, 0, sizeof vis);
	memset(visc, 0, sizeof visc);
	memset(d, 0x3f, sizeof d);
	d[s] = 0; vis[s] = true, visc[s] = 1;
	q.emplace(s);
	while (!q.empty()) {
		int u = q.front(); q.pop(); vis[u] = false;
		for (int i = head[u], v; i; i = e[i].nxt) {
			v = e[i].to;
			if (d[v] > d[u] + e[i].w) {
				d[v] = d[u] + e[i].w;
				if (!vis[v]) {
					++visc[v];
					if (visc[v] > n) return false;
					vis[v] = true;
					q.emplace(v);
				}
			}
		}
	}
	return true;
}
int main() {
	int t, n, m, k;
	scanf("%d", &t);
	while (t--) {
		scanf("%d%d%d", &n, &m, &k);
		memset(head, 0, sizeof head);
		for (int x, y, c; k--; ) {
			scanf("%d%d%d", &x, &y, &c);
			addedg(x, y+n, -c);
			addedg(y+n, x, c);
		}
		bool flg = true;
		for (int i = 1; i <= n+m; i++)
			if (!spfa(i, n+m)) { flg = false; break; }
		puts(flg ? "Yes" : "No");
	}
	return 0;
}