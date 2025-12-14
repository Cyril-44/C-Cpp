#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using std::max;
using std::min;
const int N = 4005, MOD = 19260817;
std::vector<int> g[N];
int a[N], n, m;
namespace S2{
	/*
5 3
1 2 3 4 5
1 2
2 3
3 4
4 5
	*/
	inline bool flag() {
		for (int i = 1; i <= n; i++)
			if (g[i].size() > 2)
				return false;
		return true;
	}
	inline void work() {
		static int chain[N];
		int u = 0;
		for (int i = 1; i <= n; i++) {
			if (g[i].size() == 1) {
				u = i;
				break;
			}
		}
		chain[1] = u;
		for (int i = 2; i <= n; i++) {
			for (const int &j : g[u]) {
				if (j == chain[i-2]) continue;
				else {
					chain[i] = j;
					break;
				}
			}
			u = chain[i];
		}
		for (int i = 1; i <= n; i++)
			debug("%d ", chain[i]);
		debug("\n");
		int mx, mn, ans = 0;
		for (int i = 1; i <= n; i++) {
			mx = 0, mn = n;
			for (int j = i; j <= n; j++) {
				mx = max(mx, a[chain[j]]);
				mn = min(mn, a[chain[j]]);
				if (mx - mn == m) ++ans;
			}
		}
		printf("%d\n", ans % MOD);
	}
}
int main() {
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= n; i++)
		scanf("%d", &a[i]);
	for (int i = 1; i < n; i++) {
		int u, v;
		scanf("%d%d", &u, &v);
		g[u].emplace_back(v);
		g[v].emplace_back(u);
	}
	if (S2::flag()) S2::work();
	else {
		puts("0");
	}
	return 0;
}