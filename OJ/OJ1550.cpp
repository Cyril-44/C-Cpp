#include <stdio.h>
#include <algorithm>
#include <functional>
#include <assert.h>
#ifdef NDEBUG
#undef NDEBUG
#endif
const int N = 1<<7, M = 25;
char s[M], n[M];
int a[N], b[N];
long long A[M];
int main() {
	scanf("%s %s", s, n);
	for (int i = 0; s[i]; i++) {
		++a[s[i]];
		++b[n[i]];
	}
	std::sort(a, a + N, std::greater<int>());
	std::sort(b, b + N, std::greater<int>());
	A[1] = 1;
	for (int i = 2; i <= 20; i++)
		A[i] = A[i-1] * i;
	long long ans = 1;
	if (std::equal(a, a + 10, b)) {
		for (int i = 1, s = 1; a[i-1]; ++i, ++s)
            if (a[i] ^ a[i-1]) ans *= A[s], s = 0;
	}
	else ans = 0;
	printf("%lld\n", ans);
	return 0;
}