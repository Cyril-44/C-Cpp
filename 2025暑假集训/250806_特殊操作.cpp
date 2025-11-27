#include <cstdio>
#include <cstring>
#include <algorithm>
#include <immintrin.h>
#include <assert.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
using std::min;
#pragma GCC target("avx2")
#pragma GCC optimize("unroll-loops")
constexpr int N = 105;
int g[N][N];
alignas(32) int f[128][128];
inline void init(const int &n, const int &s) {
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			f[i][j] = (g[i][j] ^ 0x3f3f3f3f) ? g[i][j] + s : 0x3fffffff;
	for (int i = 1; i <= n; i++) f[i][i] = 0;
}
inline bool floyd(const int &n) {
	/* for (int k = 1; k <= n; k++)
		for (int i = 1; i <= n; i++)
			for (int j = 1; j <= n; j++)
				if ((f[i][k] ^ 0x3f3f3f3f) && (f[k][j] ^ 0x3f3f3f3f))
					f[i][j] = min(f[i][j], f[i][k] + f[k][j]); */
	for (int k = 1; k <= n; k++)
		for (int i = 1, j; i <= n; i++) {
			for (j = 1; j < 8; j++)
				f[i][j] = min(f[i][j], f[i][k] + f[k][j]);
			for (__m256i fik = _mm256_set1_epi32(f[i][k]); j+7 <= n; j += 8)
				*(__m256i*)(&f[i][j]) = _mm256_min_epi32(*(__m256i*)(&f[i][j]), _mm256_add_epi32(fik, *(__m256i*)(&f[k][j])));
			for (; j <= n; ++j)
				f[i][j] = min(f[i][j], f[i][k] + f[k][j]);
		}
	/* for (int k = 1; k <= n; k++)
		for (int i = 1; i <= n; i++) {
			__m256i mtp = _mm256_set1_epi32(f[i][k]);
			int j = 1;
			while (j+7 <= n) {
				__m256i fij = _mm256_loadu_si256((__m256i*)(f[i] + j));
	            __m256i fkj = _mm256_loadu_si256((__m256i*)(f[k] + j));
	            __m256i res = _mm256_min_epi32(fij, _mm256_add_epi32(mtp, fkj));
	            _mm256_storeu_si256((__m256i*)(f[i] + j), res);
	            j += 8;
			}
			for (; j <= n; ++j)
				f[i][j] = min(f[i][j], f[i][k] + f[k][j]);
		} */
	for (int i = 1; i <= n; i++)
        if (f[i][i] < 0 && f[1][i] <= (int)(1e9) && f[i][n] <= (int)(1e9)) return false;
	return true;
}
namespace FastIO {
	char buf[1 << 21], *p1(nullptr), *p2(nullptr);
	inline char getchar() { return (p1 == p2) && (p2 = (p1 = buf) + fread(buf, sizeof(char), sizeof buf, stdin), p1 == p2) ? EOF : *p1++; }
	inline void in(int &x) {
		char ch = getchar(); register int f = 1;
		while ((ch < '0' || ch > '9') && (ch ^ '-')) ch = getchar();
		if (ch == '-') ch = getchar(), f = -1;
		for (x = 0; ch >= '0' && ch <= '9'; ch = getchar())
			x = (x << 3) + (x << 1) + (ch ^ '0');
		x *= f;
	}
} using FastIO::in;
int main() {
	int t, n, m;
	in(t);
	while (t--) {
		memset(g, 0x3f, sizeof g);
		in(n), in(m);
		for (int u, v, w; m--; ) {
			in(u), in(v), in(w);
			g[u][v] = min(g[u][v], w);
		}
		int l = (int)(-1e5), r = (int)(1e5), mid;
		while (l <= r) {
			mid = l + (r - l >> 1);
			init(n, mid);
			if (floyd(n) && f[1][n] >= 0) r = mid - 1;
			else l = mid + 1;
		}
		init(n, l);
		floyd(n);
		printf("%d\n", (f[1][n] < 0 || f[1][n] == 0x3fffffff) ? -1 : f[1][n]);
	}
	return 0;
}
