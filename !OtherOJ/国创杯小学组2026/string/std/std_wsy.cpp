#include <bits/stdc++.h>

using namespace std;

#define ull unsigned long long

const int N = 5000005;
const short B = 131;

char s[N];
ull h1[N], h2[N], pw[N];
int tmp[128], pre[N], f[N], t1[N], t2[N];

inline void chkmn(int &x, int y) { if (y < x) x = y; }

inline bool check(int l, int r) {
	// printf("HAVK: %d %d\n", l, r);
	int len = (r - l + 1) >> 1;
	if (t1[l + len] != t1[l] || t2[r - len + 1] != t2[r]) return 0;
	return h1[l + len - 1] - h1[l - 1] * pw[len] == h2[r - len + 1] - h2[r + 1] * pw[len];
}

int main() {
	int T;
	scanf("%d", &T);
	pw[0] = 1;
	for (int i = 1; i < N; i++) pw[i] = pw[i - 1] * B;
	while (T--) {
		scanf("%s", s + 1);
		int n = strlen(s + 1);
		for (int i = 1; i <= n; i++) {
			h1[i] = h1[i - 1] * B + s[i];
			t1[i] = t1[i - 1] + (s[i] < s[i - 1]);
		}
		for (int i = n; i >= 1; i--) {
			h2[i] = h2[i + 1] * B + s[i];
			t2[i] = t2[i + 1] + (s[i] < s[i + 1]);
		}
		memset(tmp, 0, sizeof(tmp));
		for (int i = 1; i <= n; i++) {
			pre[i] = tmp[s[i]];
			tmp[s[i]] = i;
		}
		f[0] = 0;
		// printf("??: %d\n", check(2, 5));
		for (int i = 1; i <= n; ) {
			int j = i;
			while (s[j] == s[i]) j++;
			int p = pre[i], now = f[i - 1];
			for (; i < j; i++) {
				f[i] = 0x3f3f3f3f;
				if (p && check(p, i)) chkmn(f[i], f[p - 1] + 1), p = pre[p];
                else p = 0;
				chkmn(now, f[i - 1]);
				chkmn(f[i], now + 1);
			}
			// printf("the range is: %d %d\n", i, j - 1);
			i = j;
		}
		printf("%d\n", f[n]);
		// for (int i = 1; i <= n; i++) {
		// 	printf("%d ", f[i]);
		// }
		// printf("\n");
	}
	
	return 0;
}