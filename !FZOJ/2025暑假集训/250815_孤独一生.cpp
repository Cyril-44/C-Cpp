#include <cstdio>
#include <cstring>
#include <algorithm>
using std::min;
constexpr int N = 500005;
constexpr long long INF = 0x3f3f3f3f3f3f3f3fLL;
int a[N];
struct Dat1 {
	static constexpr int N = 105;
	long long f[2][N][N][N];
	inline void work(const int &n) {
		memset(f[1], 0x3f, (sizeof f) >> 1);
		f[1][1][1][0] = a[1], f[1][0][0][1] = a[1];
		for (int i = 1; i < n; i++) {
			memset(f[i&1^1], 0x3f, (sizeof f) >> 1);
			for (int j = 0; j <= i; j++)
				for (int k = 0; k <= i; k++)
					for (int l = 0; l <= i; l++)
						if (f[i&1][j][k][l] ^ INF) {
							f[i&1^1][j+1][i+1][l] = min(f[i&1^1][j+1][i+1][l], f[i&1][j][k][l] + abs(a[i+1] - a[k]));
							f[i&1^1][j][k][i+1] = min(f[i&1^1][j][k][i+1], f[i&1][j][k][l] + abs(a[i+1] - a[l]));
						}
		}
		long long ans = INF;
		for (int j = 0; j <= n; j++)
			for (int k = 0; k <= n; k++)
				for (int l = 0; l <= n; l++)
					ans = min(ans, f[n&1][j][k][l]);
		printf("%lld\n", ans);
	}
};
struct Dat2 {
	static constexpr int N = 5005;
	long long f[2][N][2];
	inline void work(const int &n) {
		memset(f[1], 0x3f, (sizeof f) >> 1);
		f[1][0][0] = f[1][0][1] = a[1];
		for (int i = 1; i < n; i++) {
			auto f1 = f[i&1], f2 = f[i&1^1];
			memset(f2, 0x3f, (sizeof f) >> 1);
			for (int j = 0; j <= i; j++) {
				if (f1[j][0] ^ INF) {
					f2[j][0] = min(f2[j][0], f1[j][0] + abs(a[i+1] - a[i]));
					f2[i][1] = min(f2[i][1], f1[j][0] + abs(a[i+1] - a[j]));
				}
				if (f1[j][1] ^ INF) {
					f2[j][1] = min(f2[j][1], f1[j][1] + abs(a[i+1] - a[i]));
					f2[i][0] = min(f2[i][0], f1[j][1] + abs(a[i+1] - a[j]));
				}
			}
		}
		long long ans = INF;
		for (int j = 0; j <= n; j++)
			ans = min(ans, min(f[n&1][j][0], f[n&1][j][1]));
		printf("%lld\n", ans);
	}
};
struct Dat {
	struct Unique {
		int a[N], tot;
		inline void init(int *arr, int n) {
			memcpy(a, arr, sizeof(int) * (n+1));
			std::sort(a, a+1 + n);
			tot = std::unique(a, a+1 + n) - a;
		}
		inline int operator()(const int &x) const {
			return std::lower_bound(a, a+tot, x) - a + 1;
		}
	} u;
	template <bool Inv = false>
	struct BIT {
		long long tr[N];
		BIT () { memset(tr, 0x3f, sizeof tr); }
		inline void upd(int p, const long long &x) {
			for (++p, p = Inv ? 100001 - p + 1 : p; p <= 100001; p += p & -p) tr[p] = min(tr[p], x);
		}
		inline long long operator()(int p) {
			long long res = INF;
			for (++p, p = Inv ? 100001 - p + 1 : p; p > 0; p -= p & -p) res = min(res, tr[p]);
			return res;
		}
	};
	BIT<false> pre;
	BIT<true> suf;
	inline void work(const int &n) {
		u.init(a, n);
		static long long f[N], s[N];
		for (int i = 1; i <= n; i++)
			s[i] = s[i-1] + abs(a[i] - a[i-1]);
		long long ans = INF;
		for (int i = 1; i <= n; i++) {
			f[i] = s[i-1] + a[i];
			int j = u(a[i]);
			f[i] = min(f[i], min(suf(j) + s[i-1] - a[i], pre(j) + s[i-1] + a[i]));
			j = u(a[i-1]);
			suf.upd(j, f[i] - s[i] + a[i-1]);
			pre.upd(j, f[i] - s[i] - a[i-1]);
			ans = min(ans, f[i] + s[n] - s[i]);
		}
		printf("%lld\n", ans);
	}
};
int main() {
	int n; scanf("%d", &n);
	for (int i = 1; i <= n; i++)
		scanf("%d", a + i);
//	(new Dat1) -> work(n);
//	(new Dat2) -> work(n);
	(new Dat) -> work(n);
	return 0;
}