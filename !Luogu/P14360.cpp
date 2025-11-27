#include <stdio.h>
#include <algorithm>
constexpr int N = 5005, MOD = 998244353;
int a[N], f[N];
inline void add(int &x, int y) { if ((x += y) >= MOD) x -= MOD; } 
int main() {
	int n;
	scanf("%d", &n);
	for (int i = 1; i <= n; i++)
		scanf("%d", &a[i]);
	std::sort(a+1, a+1 + n);
	int Mx = a[n] + 1;
	f[0] = 1;
	int ans = 0;
	for (int i = 1; i <= n; i++) {
		for (int j = a[i] + 1; j <= Mx; j++)
			add(ans, f[j]);
		for (int j = Mx; j >= 0; j--)
			add(f[std::min(Mx, j + a[i])], f[j]);
	}
	printf("%d\n", ans);
	return 0;
}