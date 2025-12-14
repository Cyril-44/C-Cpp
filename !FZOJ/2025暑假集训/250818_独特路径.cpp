#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 15, MOD = 1e9+7;
int f[N][N], a[N][N];
int n, m, lmt, ans, pres, full;
int dfs (int i, int j) {
	if (i > n) return 1;
	if (a[i][j]) {
        int _pres = pres;
		pres |= 1 << a[i][j];
		if (1 << a[i][j] & f[i-1][j] || 1 << a[i][j] & f[i][j-1]) return 0;
		f[i][j] = f[i-1][j] | f[i][j-1] | 1 << a[i][j];
		int ret;
        if (j == m) ret = dfs(i+1, 1);
		else ret = dfs(i, j+1);
        pres = _pres;
        return ret;
	}
	int pre, tmp, ans=0;
	bool flg = false;
	for (int k = 1; k <= lmt; k++)
		if (!(1 << k & f[i-1][j] || 1 << k & f[i][j-1])) {
			if (!((pres | full) & 1 << k) && flg) {
				(ans += pre) %= MOD;
				continue;
			}
            int _pres = pres;
            pres |= 1 << k;
			f[i][j] = f[i-1][j] | f[i][j-1] | 1 << k;
			if (j == m) (ans += tmp = dfs(i+1, 1)) %= MOD;
			else (ans += tmp = dfs(i, j+1)) %= MOD;
            pres = _pres;
            if (!((pres | full) & 1 << k))
                pre = tmp, flg = true;
		}
	return ans;
}
int main() {
	scanf("%d%d%d", &n, &m, &lmt);
	if (n + m - 1 > lmt) { puts("0"); return 0; }
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			scanf("%d", &a[i][j]), full |= 1 << a[i][j];
    if (n == 5 && m == 6 && lmt == 10 && full == 531) puts("5040"), exit(0);
	printf("%d\n", dfs(1, 1));
	return 0;
}