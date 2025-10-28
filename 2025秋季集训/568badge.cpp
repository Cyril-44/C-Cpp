#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

const int N = 500005;

int d[N], l[N], r[N], B = 700, n, q;
vector<int> e[N];
int s[N], sb[N], id[N], a[N], ans[N];

struct Node {
	int l, r, id;
};
vector<Node> v[N];

void add(int x) {
	for (int i = x; i <= min(n, id[x] * B); i++) s[i]++;
	for (int i = id[x]; i <= id[n]; i++) sb[i]++;
}
int query(int l, int r) {
	return s[r] + sb[id[r] - 1] - sb[id[l - 1] - 1] - s[l - 1];
}

int main() {
//	system("fc triple.out triple2.ans");
	freopen("badge.in", "r", stdin);
	freopen("badge.out", "w", stdout);
	scanf("%d%d", &n, &q);
	
	for (int i = 1; i <= n; i++) {
		scanf("%d%d", &l[i], &r[i]);
		e[r[i]].push_back(l[i]);
	}
	
	B = sqrt(n);
	
	for (int k = 1; k <= q; k++) {
		int m;
		scanf("%d", &m);
		for (int i = 1; i <= m; i++) {
			scanf("%d", &a[i]);
		}
		if (m >= B) {
			int res = 0;
			for (int i = 1; i <= n; i++) d[i] = 0;
			for (int i = 1; i <= m; i++) d[a[i]] ^= 1;
			for (int i = 1; i <= n; i++) d[i] ^= d[i - 1];
			for (int i = 1; i <= n; i++) res += d[r[i]] ^ d[l[i] - 1];
			ans[k] = res;
//			printf("DD: %d\n", k);
		} else {
			sort(a + 1, a + 1 + m);
			for (int i = m; i >= 1; i--) {
				for (int j = i; j >= 1; j -= 2) {
					if (i != m) v[a[i + 1]].push_back({a[j - 1] + 1, a[j], -k});
					v[a[i]].push_back({a[j - 1] + 1, a[j], k});
				}
			}
		}
	}
	
	for (int i = 1; i <= n; i++) id[i] = (i - 1) / B + 1;
	
	for (int i = n; i >= 1; i--) {
		for (auto x : e[i]) add(x);
		for (auto p : v[i]) {
//			printf("%d %d %d\n", p.id, p.l, p.r);
			p.id > 0 ? ans[p.id] += query(p.l, p.r) : ans[-p.id] -= query(p.l, p.r);
		}
	}
	
	for (int i = 1; i <= q; i++) printf("%d\n", ans[i]);
	
	return 0;
}
/*
1
-1 2 -8
2 4 8
*/