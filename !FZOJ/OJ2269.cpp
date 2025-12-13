#include <stdio.h>
#include <vector>
#include <algorithm>
const int K = 105;
struct Attack {
	int t, op, x;
} a[K];
int main() {
	// freopen("guns.in", "r", stdin);
	// freopen("guns.out", "w", stdout);
	int t, n, m, k, add;
	scanf("%d", &t);
	while (t--) {
		scanf("%d%d%d", &n, &m, &k);
		std::vector<std::vector<int> > f(n+1, std::vector<int>(m+1));
		std::vector<std::vector<std::vector<int> > > al(n+1, std::vector<std::vector<int> >(m+1));
		std::vector<int> h[n+1], v[m+1]; // Horizonal Verticial
		for (int i = 0; i < k; i++) {
			scanf("%d%d%d", &a[i].t, &a[i].op, &a[i].x);
			switch (a[i].op) {
			case 1:
				h[a[i].x].push_back(a[i].t);
				break;
			case 2:
				v[a[i].x].push_back(a[i].t);
				break;
			}
		}
		for (int i = 0; i <= n; i++)
			for (int j = 0; j <= m; j++) {
				for (int k : h[i]) al[i][j].push_back(k);
				for (int k : v[j]) al[i][j].push_back(k);
				std::sort(al[i][j].begin(), al[i][j].end());
			}
		std::vector<int>::iterator it;
		for (int i = 0; i <= n; i++)
			for (int j = 0; j <= m; j++) {
				if (i==0 && j==0) continue;
				f[i][j] = 20001;
				if (j > 0) {
					for (add = 1; (true && add <= 20000) && (
						it = std::lower_bound(
							al[i][j].begin(), 
							al[i][j].end(), 
							f[i][j-1] + add
						), it != al[i][j].end() && *it == f[i][j-1] + add
					); ++add) if (false && (it = std::lower_bound(
							al[i][j-1].begin(), 
							al[i][j-1].end(), 
							f[i][j-1] + add-1
						), it != al[i][j-1].end() && *it == f[i][j-1] + add-1)) {
						add = 20000;
						break;
					}
					if (it = std::lower_bound(
							al[i][j-1].begin(), 
							al[i][j-1].end(), 
							f[i][j-1] + add-1
						), it != al[i][j-1].end() && *it == f[i][j-1] + add-1) {
						add = 20000;
					}
					f[i][j] = std::min(f[i][j], f[i][j-1] + add);
				}
				if (i > 0) {
					for (add = 1; (true && add <= 20000) && (
						it = std::lower_bound(
							al[i][j].begin(), 
							al[i][j].end(), 
							f[i-1][j] + add
						), it != al[i][j].end() && *it == f[i-1][j] + add
					); ++add) if (false && (it = std::lower_bound(
							al[i-1][j].begin(), 
							al[i-1][j].end(), 
							f[i-1][j] + add-1
						), it != al[i-1][j].end() && *it == f[i-1][j] + add-1)) {
						add = 20000;
						break;
					}
					if (it = std::lower_bound(
							al[i-1][j].begin(), 
							al[i-1][j].end(), 
							f[i-1][j] + add-1
						), it != al[i-1][j].end() && *it == f[i-1][j] + add-1) {
						add = 20000;
					}
					f[i][j] = std::min(f[i][j], f[i-1][j] + add);
				}
			}
		printf("%d\n", f[n][m] < 20000 ? f[n][m] : -1);
	}
	return 0;
}