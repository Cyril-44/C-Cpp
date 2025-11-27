#include <stdio.h>
#include <algorithm>
#include <limits>
constexpr int N = 300005;
int a[N];
int main() {
	int n;
	scanf("%d", &n);
	for (int i = 1; i <= n; i++)
		scanf("%d", &a[i]);
	if (n == 1) return printf("%d\n", a[1]), 0;
	std::sort(a+1, a+1 + n);
	long long ans = a[n] - a[1];
	for (int i = 2; i < n; i++)
		ans += std::abs(a[i]);
	printf("%lld\n", ans);
	return 0;
}