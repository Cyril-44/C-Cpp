#include <stdio.h>
int main() {
	freopen("xor.in", "r", stdin);
	freopen("xor.out", "w", stdout);
#ifdef DEBUG
	freopen("xor.dbg", "w", stderr);
#endif
	int T;
	long long l, r;
	scanf("%d", &T);
	while (T--) {
		scanf("%lld%lld", &l, &r);
		if (l == r) {
			fprintf(stderr, "------\n");
			puts("1");
			continue;
		}
		int back = 64 - __builtin_clzll(r);
		long long mid = l ^ r;
		int front = 64 - __builtin_clzll((1ll << back) - mid);
		if (front == back) printf("%lld\n", 1ll << back);
		else if (mid < (1ll << front)) printf("%lld\n", 1ll << front);
		else printf("%lld\n", (1ll << front) + (1ll << back) - mid);
		fprintf(stderr, "%lld %lld %lld\n", (1ll << front)-1, mid, (1ll << back)-1);
	}
	return 0;
}