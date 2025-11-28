#include <stdio.h>
const int N = 100005;
char s[N];
inline int abs(const int &x) { return x < 0 ? -x : x; }
inline int min(const int &x, const int &y) {
	return x < y ? x : y;
}
int main() {
	int n, p, i, r, mn, mx, cur, ans = 0;
	scanf("%d%d %s", &n, &p, s);
	i = p > (n>>1) ? n>>1|n&1 : 0;
	r = p > (n>>1) ? n : n>>1;
//	printf("%d %d %d\n", i, r, p);
	for (mn = mx = -1; i < r; i++) {
		cur = min(abs(s[i] - s[n-i-1]), 26 - abs(s[i] - s[n-i-1]));
		if (abs(s[i] - s[n-1-i])) {
//			printf("%d\n", cur);
			ans += cur;
			if (mn == -1) mn = i;
			mx = i;
		}	
	}
//	printf("%d %d\n", mn, mx);
	ans += min(abs(p-1-mn), abs(mx-p+1)) + mx-mn;
	if (mx == -1) ans = 0;
	printf("%d\n", ans);
	return 0;
}