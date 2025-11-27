#include <cstdio>
#include <cstring>
#include <algorithm>
#pragma GCC optimize(2)
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast,unroll-loops")
const int N = 20;
int n[N];
long long ans;
inline void work(const int &fn) {
	static long long f[N][15 * N][15 * N][2];
//		i位数 数字和为j 取模为k 0< 1=
	memset(f, 0, sizeof f);
	f[0][0][0][1] = 1;
	for (register int i = 1; i <= n[0]; i++) { // 当前位数 
		for (register int j = 0; j <= fn; j++) { // 当前数位和 
			for (register int k = 0; k < fn; k++) { // 前一个mod值
				for (register int l = 0; l < 10 && l <= j; l++) { // 当前数位
					// register int m = (k * 10 + l) % fn; // 当前mod值
					f[i][j][(k * 10 + l) % fn][0] += f[i-1][j-l][k][0];
				}
				for (register int l = 0; l < n[i] && l <= j; l++)
					f[i][j][(k * 10 + l) % fn][0] += f[i-1][j-l][k][1];
				if (j >= n[i]) f[i][j][(k * 10 + n[i]) % fn][1] += f[i-1][j-n[i]][k][1];
			}
		}
	}
	ans += f[n[0]][fn][0][0] + f[n[0]][fn][0][1];
}
int main() {
	char s[N];
	scanf("%s", s);
	n[0] = strlen(s);
	for (int i = 1; i <= n[0]; i++)
		n[i] = s[i - 1] ^ '0';
	for (int k = 1; k <= 9 * n[0]; k++)
		work(k);
	printf("%lld\n", ans);
	return 0;
}