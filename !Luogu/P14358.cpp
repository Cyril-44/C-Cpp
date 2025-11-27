#include <stdio.h>
#include <algorithm>
#include <functional>
constexpr int N = 105;
int a[N];
int main() {
	int n, m;
	scanf("%d%d", &n, &m);
	for (int i = 0; i < n*m; i++)
		scanf("%d", &a[i]);
	int r = a[0];
	std::sort(a, a+n*m, std::greater<int>());
	int x = 1, y = 1;
	for (int i = 0; i < n * m; i++) {
		if (a[i] == r) {
			printf("%d %d\n", x, y);
			return 0;
		}
		if (x & 1) ++y; else --y;
		if (y < 1) ++x, y = 1;
		if (y > n) ++x, y = n;
	}
}