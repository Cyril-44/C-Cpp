#include <bits/stdc++.h>
constexpr int N = 55;
int a[N];
struct Node {
	int v, w;
	inline bool operator<(const Node &rhs) const {
		return v * rhs.w > rhs.v * w || v * rhs.w == rhs.v * w && v > rhs.v;
	}
} b[N];
int main() {
	int c, T;
	scanf("%d%d", &c, &T);
	while (T--) {
		int n, m;
		scanf("%d%d", &n, &m);
		for (int i = 0; i < n; i++)
			scanf("%d", &a[i]);
		int cnt = 0;
		for (unsigned s = 0; !(s >> n); s++) {
			for (int i = 0; i < n; i++)
				b[i].v = a[i], b[i].w = (s >> i & 1) + 1;
			std::stable_sort(b, b+n);
			int tot = m, val = 0;
			for (int i = 0; i < n; i++)
				if (tot >= b[i].w) {
					tot -= b[i].w;
					val += b[i].v;
				}
			int f[N]{};
			memset(f, -0x3f, sizeof f);
			f[0] = 0;
			for (int i = 0; i < n; i++)
				for (int j = m; j >= b[i].w; j--)
					f[j] = std::max(f[j], f[j - b[i].w] + b[i].v);
			if (val != *std::max_element(f, f+m+1)) {
				++cnt;
				for (int i = 0; i < n; i++)
					printf("%d/%d ", a[i], (s>>i&1)+1);
				putchar('\n');
			}
		}
		printf("%d\n", (1 << n) - cnt);
	}
	return 0;
}