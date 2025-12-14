#include <cstdio>
#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <cassert>
#if !defined(ONLINE_JUDGE) || 1
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug(...) 0
#endif
using std::min;
constexpr int N = 10005;
constexpr long long INF = 0x3f3f3f3f3f3f3f3fLL;
struct Node {
	int l, r, s;
	inline bool operator< (const Node &rhs) const {
		return r < rhs.r || r == rhs.r && l < rhs.l;
	}
} a[N];
struct Unique {
	int a[N << 1], tot;
	inline void init(Node *arr, int m, int e, int n) {
		for (int i = 1; i <= n; i++)
			a[2*i+1] = arr[i].l, a[2*i+2] = arr[i].r;
		a[1] = m, a[2] = e;
		std::sort(a+1, a+1 + (2*n+2));
		tot = std::unique(a+1, a+1 + (2*n+2)) - a;
	}
	inline int operator()(const int &x) const {
		return std::lower_bound(a+1, a+tot, x) - a;
	}
} u;
typedef long long i64;
struct SegTree {
	i64 mn[N << 3];
	inline void init() {
		memset(mn, 0x3f, sizeof mn);
	}
	inline void pushup(const i64 &u) { mn[u] = min(mn[u<<1], mn[u<<1|1]); }
	inline void upd(const i64 &x, const int &p, int u=1, int l=1, int r=::u.tot) {
		if (l > r) throw std::runtime_error("Fuck You! Range: [" + std::to_string(l) + ", " + std::to_string(r) + "] What the fuck!!!!\n");
		if (l == r) {
			mn[u] = min(mn[u], x);
			return;
		}
		int mid = l + (r - l >> 1);
		if (p <= mid) upd(x, p, u<<1, l, mid);
		else upd(x, p, u<<1|1, mid+1, r);
		pushup(u);
	}
	inline i64 query(const int &ql, const int &qr, int u=1, int l=1, int r=::u.tot) const {
		if (l > r) throw std::runtime_error("Fuck You! Range: [" + std::to_string(l) + ", " + std::to_string(r) + "] What the fuck!!!!\n");
		if (ql <= l && r <= qr) return mn[u];
		int mid = l + (r - l >> 1);
		i64 res = INF;
		if (ql <= mid) res = min(res, query(ql, qr, u<<1, l, mid));
		if (mid < qr) res = min(res, query(ql, qr, u<<1|1, mid+1, r));
		return res;
	}
	inline i64 operator()(const int &l, const int &r) const { return query(l, r); }
} f;
int main() {
	int n, m, e;
	scanf("%d%d%d", &n, &m, &e);
	for (int i = 1; i <= n; i++) {
		scanf("%d%d%d", &a[i].l, &a[i].r, &a[i].s);
	}
	u.init(a, m, e, n);
	std::sort(a+1, a+1 + n);
	f.init();
	for (int i = 1; i <= n; i++) {
		int l = u(a[i].l), r = u(a[i].r);
		if (l == u(m)) f.upd(a[i].s, r);
		else f.upd(f(l-1, r) + a[i].s, r);
	}
	for (int i = 1; i <= n; i++) {

	}
	if (f(u(e), u(e)) < INF) printf("%lld\n", f(u(e), u(e)));
	else puts("-1");
	return 0;
}