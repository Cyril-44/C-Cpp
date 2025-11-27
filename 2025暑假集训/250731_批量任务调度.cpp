#include <cstdio>
#include <cstring>
#include <algorithm>
using std::min;
constexpr int N = 10005;
int a[N], t[N], f[N];
long long g[N];
namespace Que {
	int l(1), r(0);
	int q[N];
	inline double k(const int &x, const int &y) { return double(g[x] - g[y]) / (t[x] - t[y]); }
	inline void push(const int &x) { while (l < r && k(q[r-1], q[r]) >= k(q[r], x)) --r; q[++r] = x; }
	inline void pop(const int &x) { while (l < r && k(q[l], q[l+1]) < x) ++l; }
	inline int top() { return q[l]; }
} using Que::push; using Que::pop; using Que::top;
int main() {
	int n, s;
	scanf("%d%d", &n, &s);
	for (int i = 1; i <= n; i++)
		scanf("%d%d", t+i, f+i);
	for (int i = n; i >= 1; i--)
		t[i] += t[i+1], f[i] += f[i+1];
	memset(g, 0x3f, sizeof g);
	g[n+1] = 0;
	push(n+1);
	for (int i = n; i >= 1; i--) {
		pop(f[i]);
		g[i] = g[top()] + f[i] * (t[i] - t[top()] + s);
		push(i);
	}
	printf("%lld\n", g[1]);
	return 0;
}