#include <cstdio>
#include <cstring>
#include <algorithm>
#include <queue>
using std::max;
namespace FastIO {
	char buf[1 << 20], *p1(nullptr), *p2(nullptr);
	inline char getchar() { return (p1 == p2) && (p2 = (p1 = buf) + fread(buf, sizeof(char), sizeof buf, stdin), p1 == p2) ? EOF : *p1++; }
	inline void in(int &x) {
		char ch = getchar();
		while (ch < '0' || ch > '9') ch = getchar();
		for (x = 0; ch >= '0' && ch <= '9'; ch = getchar())
			x = (x << 3) + (x << 1) + (ch ^ '0');
	}
} using FastIO::in;
constexpr int N = 100005, M = 600005;
struct Edge {
	int to, nxt, w;
} e[M];
int head[N];
inline void addedg(const int &fr, const int &to, const int &w) {
	e[++head[0]] = Edge {to, head[fr], w};
	head[fr] = head[0];
}
long long d[2][N];
inline void dij(long long *f, const int &n, const int &s) {
	static bool vis[N];
	memset(f, 0x3f, sizeof(long long) * (n+1));
	memset(vis, 0, n+1);
	std::priority_queue<std::pair<long long,int>, std::vector<std::pair<long long,int>>, std::greater<std::pair<long long,int>>> q;
	f[s] = 0;
	q.emplace(0, s);
	while (!q.empty()) {
		int u = q.top().second; q.pop();
		if (vis[u]) continue;
		vis[u] = true;
		for (int i = head[u], v; i; i = e[i].nxt) {
			v = e[i].to;
			if (!vis[v] && f[v] > f[u] + e[i].w) {
				f[v] = f[u] + e[i].w;
				q.emplace(f[v], v);
			}
		}
	}
}
long long x[N], y[N];
int main() {
#ifndef ONLINE_JUDGE
	freopen("E.in", "r", stdin);
#endif
	int t, n, m;
	in(t);
	while (t--) {
		in(n), in(m);
		memset(head, 0, sizeof(int) * (n+1));
		for (int u, v, c; m--; ) {
			in(u), in(v), in(c);
			addedg(u, v, c);
			addedg(v, u, c);
		}
		dij(d[0], n, 1);
		dij(d[1], n, 2);
		for (int i = 1; i <= n; i++)
			x[i] = d[0][i] + d[1][i], y[i] = d[0][i] - d[1][i];
		std::sort(x+1, x+1 + n);
		std::sort(y+1, y+1 + n);
		long long sum = 0;
		for (int i = 1, mid = (1+n) >> 1; i <= n; i++)
			sum += llabs(x[i] - x[mid]) + llabs(y[i] - y[mid]);
		printf("%.8lf\n", sum * 0.5 / n);
	}
	return 0;
}