#include <stdio.h>
#include <string.h>
const int N = 100005, M = 1005;
int c[30][2], fa[30];
// 0=首、入度 1=尾、出度 
inline int getfa(int u) {
	register int r = fa[u], x;
	while (r ^ fa[r]) r = fa[r];
	while (fa[u] ^ r) x = fa[u], fa[u] = r, u = x;
	return r;
}
int main() {
	int t, n, a0, a1;
	char s[M];
	scanf("%d", &t);
	while (t--) {
		memset(c, 0, sizeof c);
		memset(fa, 0, sizeof fa);
		scanf("%d", &n);
		for (int i = 1; i <= n; i++) {
			scanf("%s", s);
			a0 = *s - 'a' + 1;
			a1 = *(s + strlen(s) - 1) - 'a' + 1;
			// a0 -> a1
			++c[a0][1];
			++c[a1][0];
			if (!fa[a0]) fa[a0] = a0;
			if (!fa[a1]) fa[a1] = a1;
			fa[getfa(a0)] = getfa(a1);
		}
		int cnt1 = 0, cnt2 = 0, cnt3 = 0;
		for (int i = 1; !cnt3 && i <= 26; i++) 
            if (fa[i]) {
                if (c[i][0] - c[i][1] == 1) // 入度=出度+1
                    ++cnt1;
                else if (c[i][1] - c[i][0] == 1) // 入度=出度-1
                    ++cnt2;
                else if (c[i][1] - c[i][0])
                    ++cnt3;
		    }
        if (cnt3 ^ -1) {
		    for (int i = 1; i <= 26; i++)
		    	if (fa[i] == i) ++cnt3;
		    if (cnt3 == 1 && (cnt1 == 0 && cnt2 == 0 || cnt1 == 1 && cnt2 == 1)) puts("Yes");
		    else puts("No");
        }
        else puts("No");
	}
	return 0;
}