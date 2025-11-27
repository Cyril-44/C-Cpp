#include <stdio.h>
#include <algorithm>
const int N = 100005;
struct Node {
	int a, b, c;
} a[N];
int main() {
	int x, y, z, n;
	scanf("%d%d%d", &x, &y, &z);
	n = x + y + z;
	for (int i = 0; i < n; i++)
		scanf("%d%d%d", &a[i].a, &a[i].b, &a[i].c);
	std::sort(a, a + n, [](const Node &x, const Node &y) { return x.a - x.b < y.a - y.b; });
	std::sort(a, a + y+z, [](const Node &x, const Node &y) { return x.b - x.c < y.b - y.c; });
	std::sort(a + y, a + n, [](const Node &x, const Node &y) { return x.a - x.c < y.a - y.c; });
	long long ans = 0;
	for (int i = 0; i < y; i++)
		ans += a[i].b;
	for (int i = z; i < z+y; i++)
		ans += a[i].c;
	for (int i = z+y; i < n; i++)
		ans += a[i].a;
	printf("%lld\n", ans);
	return 0;
}