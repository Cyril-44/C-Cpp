#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <numeric>
constexpr int N = 300005;
long long a[N];
bool f[N];
long long pre[N];
int s2[1005][6], cnts2=0, _sig_s2 = []() {
	int a[6]; std::iota(a, a+6, 0);
	do if (a[0] < a[3] && std::is_sorted(a, a+3) && std::is_sorted(a+3, a+6))
		memcpy(s2[++cnts2], a, sizeof a);
	while (std::next_permutation(a, a+6));
	return 0;	
}();
int s3[1005][9], cnts3=0, _sig_s3 = [](){
	int a[9]; std::iota(a, a+9, 0);
	do if (a[0] < a[3] && a[3] < a[6] && std::is_sorted(a, a+3) && std::is_sorted(a+3, a+6) && std::is_sorted(a+6, a+9))
		memcpy(s3[++cnts3], a, sizeof a);
	while (std::next_permutation(a, a+9));
	return 0;
}();
inline bool check(int l, int r) {
	switch (r - l + 1) {
	case 3: return a[l] + a[l+1] > a[l+2];
	case 6: {
		for (int i = 1; i <= cnts2; i++) {
			bool flg = true;
			for (int j = 0; j < 6; j += 3)
				if (a[l+s2[i][j]] + a[l+s2[i][j+1]] <= a[l+s2[i][j+2]]) flg = false;
			if (flg) 
				// fprintf(stderr, "[%d, %d] %d%d%d%d%d%d\n", l, r, s2[i][0], s2[i][1], s2[i][2], s2[i][3], s2[i][4], s2[i][5]);
				return true;
		}
		break;
	}
	case 9: {
		for (int i = 1; i <= cnts3; i++) {
			bool flg = true;
			for (int j = 0; j < 9; j += 3)
				if (a[l+s3[i][j]] + a[l+s3[i][j+1]] <= a[l+s3[i][j+2]]) flg = false;
			if (flg) return true;
		}
		break;
	}
	}
	return false;
}
inline long long sum(int l, int r) {
	long long sum = 0;
	for (; l <= r; ++l) sum += a[l];
	return sum;
}
signed main() {
	std::cin.tie(nullptr) -> sync_with_stdio(false);
	int n, k;
	std::cin >> n >> k;
	// scanf("%d%d", &n, &k);
	// for (int i = 1; i <= n; i++) scanf("%lld", a+i);
	for (int i = 1; i <= n; i++) std::cin >> a[i];
	std::sort(a+1, a+1 + n);
	for (int i = 3; i <= n; i++)
		f[i] = (a[i-2] + a[i-1] > a[i]);
	pre[0] = pre[1] = pre[2] = -1;
	for (int i = 3; i <= n; i++)
		pre[i] = f[i] ? a[i] + a[i-1] + a[i-2] : pre[i-1];
	int mxpos = n;
	for (; mxpos > 2 && !f[mxpos]; --mxpos);
	switch (k) {
	case 1: {
		if (mxpos <= 2) puts("-1");
		else printf("%lld\n", a[mxpos] + a[mxpos-1] + a[mxpos-2]);
		break;
	}
	case 2: {
		long long ans = -1;
		for (int i = 6; i <= n; i++)
			if (f[i] && ~pre[i-3]) ans = std::max(ans, pre[i] + pre[i-3]);
		for (int i = 6; i <= n; i++)
			if (check(i-5, i)) ans = std::max(ans, sum(i-5, i));
		printf("%lld\n", ans);
		break;
	}
	case 3: {
		long long ans = -1;
		// xxx...xxx...xxx
		for (int i = 6; i <= mxpos - 3; i++)
			if (f[i] && ~pre[i-3]) ans = std::max(ans, pre[i-3] + pre[i] + pre[mxpos]);
		// xxxxxx...xxx / xxx...xxxxxx
		for (int i = 6; i <= n; i++)
			if (check(i-5, i)) {
				if (~pre[i-6]) ans = std::max(ans, sum(i-5, i) + pre[i-6]);
				if (i <= mxpos - 3) ans = std::max(ans, sum(i-5, i) + pre[mxpos]);
			}
		// xxxxxxxxx
		for (int i = 9; i <= n; i++)
			if (check(i-8, i)) ans = std::max(ans, sum(i-8, i));
		printf("%lld\n", ans);
		break;
	}
	}
	return 0;
}