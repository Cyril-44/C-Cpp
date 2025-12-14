#include <cstdio>
#include <cstring>
#include <algorithm>
#include <numeric>
#include <immintrin.h>
#pragma GCC target("avx2", "avx")
using std::max;
constexpr int N = 10005, M = 5005, K = 505;
int a[N];
int Mx, Kk;
struct BIT {
	int tr[M + K][N + K];
	inline void upd(int p, int q, const int &x) {
		for (++q; p <= Mx; p += p & -p)
			for (int i = q; i <= Kk; i += i & -i)
				tr[p][i] = max(tr[p][i], x);
	}
	inline int operator()(int p = Mx, int q = Kk-1) const {
		int res = 0;
		for (++q; p > 0; p -= p & -p)
			for (int i = q; i > 0; i -= i & -i)
				res = max(res, tr[p][i]);
		return res;
	}
} f;
int main() {
	int n, k;
	scanf("%d%d", &n, &k);
	for (int i = 1; i <= n; i++)
		scanf("%d", &a[i]);
	Mx = *std::max_element(a+1, a+1 + n) + k, Kk = k+1;
	int ans = 0;
	for (int i = 1; i <= n; i++) {
//		printf("i = %d\n", i);
		for (int j = k; j >= 0; j--) {
//			printf("\tj = %d: %d\n", j, g(a[i] + j));
			f.upd(a[i] + j, j, f(a[i] + j, j) + 1);
		}
		ans = max(ans, f());
	}
	printf("%d\n", ans);
	return 0;
}