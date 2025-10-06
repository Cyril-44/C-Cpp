#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <vector>
constexpr int N = 100005;
std::vector<std::vector<int>> e;
namespace DiviPoint {
	int dfn[N], low[N], ts = 0; // low[u]: 不经过 fa[u] u能到的最小时间戳 
	bool flag[N];
	inline void init(int n) {
		memset(dfn, 0, sizeof(int) * (n+1));
		memset(flag, 0, sizeof(bool) * (n+1));
		ts = 0;
	}
	void dfs(int u, int fa) {
		dfn[u] = low[u] = ++ts;
		int child = 0;
		for (int v : e[u]) {
			if (!dfn[v]) {
				++child;
				dfs(v, u);
				low[u] = std::min(low[u], low[v]);
				if ((u ^ fa) && !flag[u] && low[v] >= dfn[u])
					flag[u] = true;
			}
			else if (v ^ fa)
				low[u] = std::min(low[u], dfn[v]);
		}
		if (u == fa && !flag[u] && child >= 2)
			flag[u] = true;
	}
	void debug(int n, FILE* _ = stderr) {
		fprintf(_, "Dividing points: ");
		for (int i = 1; i <= n; i++)
			if (flag[i]) fprintf(_, "%d ", i);
		fprintf(_, "\n");
	}
}
namespace SCC {
	int dfn[N], low[N], ts = 0; // low[u]: 栈中u能回溯到的最小时间戳 
	bool inSta[N];
	int sta[N], belong2[N], top[N], size[N], scnt = 0;
	inline void init(int n) {
		memset(dfn, 0, sizeof(int) * (n+1));
		scnt = 0, ts = 0;
		memset(size, 0, sizeof(int) * (n+1));
	}
	void dfs(int u) {
		dfn[u] = low[u] = ++ts;
		sta[++sta[0]] = u;
		inSta[u] = true;
		for (int v : e[u]) {
			if (v == -1) continue; // 这是割点 
			if (!dfn[v]) {
				dfs(v);
				low[u] = std::min(low[u], low[v]);
			}
			else if (inSta[v])
				low[u] = std::min(low[u], dfn[v]);
		}
		if (dfn[u] == low[u]) {
			top[++scnt] = u;
			do {
				belong2[sta[sta[0]]] = scnt;
				inSta[sta[sta[0]]] = false;
				++size[scnt];
			} while (sta[sta[0]--] != u);
		}
	}
	void debug(int n, FILE* _ = stderr) {
		fprintf(_, "SCC Counts: %d\n", scnt);
		std::vector<std::vector<int>> scc(scnt);
		for (int i = 1; i <= n; i++)
			if (!DiviPoint::flag[i]) {
				fprintf(_, "%d->%d%c", i, belong2[i], i < n ? ',' : '\n');
				scc[belong2[i] - 1].push_back(i);
			}
		for (int i = 0; i < scnt; i++) {
			fprintf(_, "SCC %d: ", i+1);
			for (int j : scc[i])
				fprintf(_, "%d ", j);
			fprintf(_, "\n");
		}
	}
}
int main() {
	int m, n;
	int cases = 0;
	while (~scanf("%d", &m) && m) {
		n = 0;
		e.clear();
		for (int u, v; m--; ) {
			scanf("%d%d", &u, &v);
			n = std::max(n, u);
			n = std::max(n, v);
			e.resize(n + 1);
			e[u].push_back(v);
			e[v].push_back(u);
		}
		DiviPoint::init(n);
		SCC::init(n);
		for (int i = 1; i <= n; i++)
			if (!DiviPoint::dfn[i]) DiviPoint::dfs(i, i);
		for (int i = 1; i <= n; i++) {
			if (!DiviPoint::flag[i])
			for (int &j : e[i])
				if (DiviPoint::flag[j]) j = -1;
		}
		for (int i = 1; i <= n; i++)
			if (!DiviPoint::flag[i] && !SCC::dfn[i])
				SCC::dfs(i);
//		DiviPoint::debug(n);
//		SCC::debug(n);
		if (SCC::scnt == 1) {
			printf("Case %d: %d %lld\n", ++cases, 2, n * (n - 1ll) / 2);
			continue;
		}
		printf("Case %d: %d ", ++cases, SCC::scnt);
		long long sum = 1;
		for (int i = 1; i <= SCC::scnt; i++)
			sum *= SCC::size[i];
		printf("%lld\n", sum);
	}
	return 0;
}
