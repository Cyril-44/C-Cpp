#include <cstdio>
#include <cstring>
#include <algorithm>
#include <limits>
using std::max;
constexpr int N = 34570;
int c[N];
long long f[N], s[N];
int main() {
	int n, t;
	scanf("%d%d", &n, &t);
	for (int i = 1; i <= n; i++)
		scanf("%d", c+i), s[i] = s[i-1] + c[i];
	for (int i = 1; i <= n; i++) {
		f[i] = std::numeric_limits<long long>::min();
		for (int j = 0; j < i; j++)
			f[i] = max(f[i], f[j] + (s[i] - s[j]) * i - t);
	}
	printf("%lld\n", f[n]);
	return 0;
}