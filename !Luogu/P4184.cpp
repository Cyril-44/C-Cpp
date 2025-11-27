#include <stdio.h>
#include <string.h>
constexpr int N = 100005, MOD = 1e9 + 7;
__attribute__((always_inline)) inline void add(int &x, int y) { if ((x += y) >= MOD) x -= MOD; }
__attribute__((always_inline)) inline int add_cp(int x, int y) { add(x, y); return x; }
__attribute__((always_inline)) inline void sub(int &x, int y) { if ((x -= y) < 0) x += MOD; }
__attribute__((always_inline)) inline int sub_cp(int x, int y) { sub(x, y); return x; }
__attribute__((always_inline)) inline void mul(int &x, int y) { x = 1ll * x * y % MOD; }
__attribute__((always_inline)) inline int mul_cp(int x, int y) { return 1ll * x * y % MOD; }
#define umax(x, y) (x) = (x) > (y) ? (x) : (y)
#define umin(x, y) (x) = (x) < (y) ? (x) : (y)
struct Point {
	int x, y;
} a[N];
int f[N], g[N]; // f->肥料覆盖 g->水源覆盖 
int sg2_g[N], sg[N];
// int h[N]{};
int sta[N]{}, cnt[N]{}; // 单调递增 
/* int qpow(int b, int n) {
	int res = 1;
	while (n) {
		if (n & 1) mul(res, b);
		mul(b, b);
		n >>= 1;
	}
	return 0;
} */
int main() {
	int n;
	scanf("%d", &n);
	for (int i = 1; i <= n; i++) {
		scanf("%d%d", &a[i].x, &a[i].y);
		++a[i].x, ++a[i].y;
	}
	memset(g, 0x3f, sizeof g);
	for (int i = 1; i <= n; i++) {
		umax(f[a[i].x], a[i].y);
		umin(g[a[i].x+1], a[i].y);
	}
	for (int i = 2; i <= n; i++)
		umin(g[i], g[i-1]);
	for (int i = 1; i <= n; i++) {
		add(sg2_g[i] = g[i] * (g[i] - 1ll) / 2 % MOD, sg2_g[i-1]);
		add(sg[i] = g[i], sg[i-1]);
	}
	for (int i = n; i >= 1; i--)
		umax(f[i], f[i+1]);
	int ans = 0, mng = 0;
	for (int i = 1; i <= n; i++) {
		while (mng < i && g[mng + 1] >= f[i]) ++mng;
		add(ans, add_cp(sub_cp(mul_cp(f[i] * (f[i] + 1ll) / 2 % MOD, i - mng), mul_cp(f[i], sub_cp(sg[i], sg[mng]))), sub_cp(sg2_g[i], sg2_g[mng])));
		/* for (int j = 1; j <= i; j++) { // g[j] -> f[i] 
			if (g[j] < f[i])
//					printf("[%d,%d]: %d %d\n", j, i, g[j], f[i]), 
				add(ans, 1ll * (f[i] - g[j]) * (f[i] - g[j] + 1) / 2 % MOD);
		} */
	}
	printf("%d\n", ans);
	return 0;
}