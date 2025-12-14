#include <cstdio>
#include <algorithm>
#include <cstring>
using std::min;
constexpr int N = 1000005;
int c[N], x[N];
typedef long long LL;
LL sp[N], spx[N], f[N];
inline LL cost(const int& l, const int& r) {
	return spx[r] - spx[l] - (x[r] - x[l]) * sp[l-1];
}
namespace Que {
    int l(1), r(0);
    int q[N];
    inline double k(const int &x, const int &y) {
        return (double)(f[x] - f[y] - spx[x+1] + spx[y+1] + ::x[x+1] * sp[x] - ::x[y+1] * sp[y]) / (sp[x] - sp[y]);
    }
    inline void push(const int &x) {
        while (l < r && k(q[r-1], q[r]) >= k(q[r], x)) --r;
        q[++r] = x;
    }
    inline void pop(const int &x) {
        while (l < r && k(q[l], q[l+1]) < x) ++l;
    }
    inline int top() { return q[l]; }
} using Que::push; using Que::pop; using Que::top;
int main() {
	int n;
	scanf("%d", &n);
	for (int i = 1, p; i <= n; i++) {
		scanf("%d%d%d", &x[i], &p, &c[i]);
		sp[i] = sp[i-1] + p;
		spx[i] = spx[i-1] + (x[i] - x[i-1]) * sp[i-1];
	}
	memset(f, 0x3f, sizeof f);
	f[0] = 0;
    push(0);
	for (int i = 1; i <= n; i++) {
		// for (int j = 1; j <= i; j++) {
		// 	f[i] = min(f[i], f[j-1] + cost(j, i) + c[i]);
		// }
        pop(x[i]);
        f[i] = f[top()] + cost(top() + 1, i) + c[i];
        push(i);
	}
	printf("%lld\n", f[n]);
	return 0;
}