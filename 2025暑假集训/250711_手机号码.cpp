#include <cstdio>
#include <cstring>
#include <algorithm>
const int N = 15;
typedef long long LL;
inline LL work(LL x) {
	int n[N] = {}, top = 0;
	while (x) n[++top] = x % 10, x /= 10;
	std::reverse(n+1, n+1 + top);
	LL f[N][2][N][3][3] = {}; // 算到第i位，0/1->比原数</= ，末位为j，0/1/2->0/4/8，0/1/2->连续1/2/3
	f[0][1][0][0][0] = 1;
	int id, equal;
	for (int i = 1; i <= top; i++) {
		for (int j = 0; j < 10; j++) // 从<原数 转移到 < 
			for (int k = 0; k < 10; k++) {
				equal = j == k;	// 当前枚举的末位和十位是否相等 
				switch (j) {	// 当前是否为4或者8 
					case 4:  id = 1; break;
					case 8:  id = 2; break;
					default: id = 0; break;
				}
/// @param l 当前0/4/8的状态 
#define forloop(t, u) \
for (int l = 0; l < 3; l++) { \
	if (equal) { \
		f[i][t][j][l][1] += f[i-1][u][k][l][0] + (l && id == l ? f[i-1][u][k][0][0] : 0); \
		f[i][t][j][l][2] += f[i-1][u][k][l][1] + (l && id == l ? f[i-1][u][k][0][1] : 0); \
	} else { \
		f[i][t][j][l][0] += f[i-1][u][k][l][0] + (l && id == l ? f[i-1][u][k][0][0] : 0); \
		f[i][t][j][l][0] += f[i-1][u][k][l][1] + (l && id == l ? f[i-1][u][k][0][1] : 0); \
		f[i][t][j][l][0] += f[i-1][u][k][l][2] + (l && id == l ? f[i-1][u][k][0][2] : 0); \
	} \
		f[i][t][j][l][2] += f[i-1][u][k][l][2] + (l && id == l ? f[i-1][u][k][0][2] : 0); \
}
				forloop(0, 0);
				if (k == n[i-1] && j < n[i]) forloop(0, 1);
			}
		int j = n[i], k = n[i-1];
		forloop(1, 1);
	}
	long long ans = f[top][1][n[top]][1][2] + f[top][1][n[top]][2][2];
	for (int j = 0; j < 10; j++)
		ans += f[top][0][j][1][2] + f[top][0][j][2][2];
	fprintf(stderr, "%lld\n", ans);
	return ans;
}
int main() {
	LL a, b;
	scanf("%lld%lld", &a, &b);
	printf("%lld\n", work(b) - work(a-1));
	return 0;
}