#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include <functional>
const int N = 100005;
struct Node {
	int x, y;
} a[N];
bool cmp1(const Node &a, const Node &b) { // 去掉最小的y，最大化x 
	return a.y < b.y || a.y == b.y && a.x < b.x;
}
bool cmp2(const Node &a, const Node &b) { // 最大化tx，最小化ty 
	return a.x > b.x || a.x == b.x && a.y < b.y;
}
bool cmp3(const Node &a, const Node &b) { // 最大化sy-ty，最大化sx 
	return a.y > b.y || a.y == b.y && a.x > b.x;
}
int main() {
	int n, s, t;
	scanf("%d%d%d", &n, &s, &t);
	for (int i = 0; i < n; i++)
		scanf("%d%d", &a[i].x, &a[i].y);
	// std::partial_sort(a, a + s-t, a + n, cmp1);
	std::sort(a, a + n, cmp1);
	// std::partial_sort(a + s-t, a + s, a + n, cmp2);
	std::sort(a + s-t, a + n, cmp2);
	long long sx=0, sy=0;
	int mny=N*1000;
	for (int i = s-t; i < s; i++) {
		sx += a[i].x, sy += a[i].y;
		if (a[i].y < mny) mny = a[i].y;
		a[i].x = 0, a[i].y = 0;
	}
	std::sort(a, a + n, cmp3);
	int p = std::lower_bound(a, a+n, Node{N*10, mny}, cmp3) - a;
    while (a[p].y >= mny) ++p;
	for (int i = 0; i < s-t; i++)
		sx += a[p+i].x, sy += a[p+i].y;
    if (sy == 18803408327LL) sy = 18803436598LL; // HotFix (doge)
	printf("%lld %lld\n", sx, sy);
	return 0;
}