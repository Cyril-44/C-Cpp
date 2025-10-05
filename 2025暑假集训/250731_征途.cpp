#include <cstdio>
#include <algorithm>
#include <cstring>
#include <vector>
#include <limits>
using std::min;
constexpr int N = 30005;
int x[N];
long long f[2][N];
namespace Que {
	int l(1), r(0);
	int q[N];
    long long *f;
    inline void updf(long long *a) { l = 1; r = 0; f = a; }
	inline double k(const int &a, const int &b) { return double(f[a] - f[b] + (long long)x[a] * x[a] - (long long)x[b] * x[b]) / (x[a] - x[b]); }
	inline void push(const int &x) { while (l < r && k(q[r-1], q[r]) >= k(q[r], x)) --r; q[++r] = x; }
	inline void pop(const int &x) { while (l < r && k(q[l], q[l+1]) < x) ++l; }
	inline int top() { return q[l]; }
} using Que::push; using Que::pop; using Que::top; using Que::updf;
int main() {
	int n, m;
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= n; i++)
		scanf("%d", x+i), x[i] += x[i-1];
    for (int i = 1; i <= n; i++) f[1][i] = (long long)x[i] * x[i];
    for (int k = 2; k <= m; k++) {
        updf(f[k & 1 ^ 1]);
        push(k - 1);
	    for (int i = k; i <= n; i++) {
			// for (int j = 0; j < i; j++)
			// 	f[i][k] = min(f[i][k], f[j][k-1] + (long long)(x[i] - x[j]) * (x[i] - x[j]));
            pop(2ll * x[i]);
            f[k & 1][i] = f[k & 1 ^ 1][top()] + (long long)(x[top()] - x[i]) * (x[top()] - x[i]);
            push(i);
        }
	}
	printf("%lld\n", m * f[m & 1][n] - (long long)x[n] * x[n]);
	return 0;
}