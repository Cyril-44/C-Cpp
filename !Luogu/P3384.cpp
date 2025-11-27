#include <cstdio>
#include <cstdlib>
#include <queue>
#include <algorithm>
#include <vector>
#ifndef ONLINE_JUDGE
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug(...) 114514
#endif
const int N = 100005;
struct Node {
	unsigned dfn, edfn;
	int val, size, hson, top, dep, fa;
	std::vector<int> sons;
} a[N];
inline void dfs1(const int &u, const int &fa) {
	a[u].fa = fa;
	a[u].size = 1;
	int mxv = -1;
	for (const int &v : a[u].sons) {
		if (v == fa) continue;
		dfs1(v, u);
		a[u].size += a[v].size;
		if (mxv == -1 || a[v].size > a[mxv].size) {
			mxv = v;
		}
	}
	debug("dfs1: %d %d\n", u, mxv);
	a[u].hson = mxv;
}
unsigned time_stamp;
inline void dfs2(const int &u, const int &fa) {
	debug("dfs2: %d %d %d\n", u, a[u].hson, fa);
	a[u].dfn = ++time_stamp;
	if (~a[u].hson) {
		a[a[u].hson].top = a[u].top;
		a[a[u].hson].dep = a[u].dep + 1;
		dfs2(a[u].hson, u);
	}
	for (const int &v : a[u].sons) {
		if (v == fa || v == a[u].hson) continue;
		a[v].top = v;
		a[v].dep = a[u].dep + 1;
		dfs2(v, u);
	}
	a[u].edfn = time_stamp;
}
int n, m, r, P=1;
struct ModInt {
	int num;
	inline ModInt(const int &x=0) : num((x % P + P) % P) {}
	inline ModInt operator-() const { return ModInt((P - num) % P); }
	inline ModInt& operator= (const int &x) { num = x % P; return *this; }
	inline ModInt& operator+=(const ModInt &x) { return *this = ModInt(num + x.num); }
	inline ModInt  operator+ (ModInt x) const { return x += *this; }
	inline ModInt& operator-=(const ModInt &x) { return *this += -x; }
	inline ModInt  operator- (ModInt x) const { return x -= *this; }
	inline ModInt& operator*=(const ModInt &x) { return *this = ModInt(1ll * num * x.num); }
	inline ModInt  operator* (ModInt x) const { return x *= *this; }
};
struct SegNode {
	ModInt sum, lazy;
} tr[N << 2];
inline void pushup(const int &u) {
	tr[u].sum = tr[u<<1].sum + tr[u<<1|1].sum;
}
inline void pushdown(const int &u, const int &range_l, const int &range_r) {
	if (tr[u].lazy.num) {
		tr[u<<1].lazy += tr[u].lazy;
		tr[u<<1].sum += tr[u].lazy * range_l;
		tr[u<<1|1].lazy += tr[u].lazy;
		tr[u<<1|1].sum += tr[u].lazy * range_r;
		tr[u].lazy = 0;
	}
}
inline void upd(const ModInt &X, const unsigned &L, const unsigned &R, int u=1, unsigned l=1, unsigned r=n) {
	debug("upd: [%d,%d]+=%d %d %d %d\n", L, R, X.num, u, l, r);
	if (L <= l && r <= R) {
		tr[u].sum += X * (r - l + 1);
		tr[u].lazy += X;
		return;
	}
	unsigned mid = l + r >> 1;
	pushdown(u, mid - l + 1, r - mid);
	if (L <= mid) upd(X, L, R, u<<1, l, mid);
	if (R > mid) upd(X, L, R, u<<1|1, mid+1, r);
	pushup(u);
}
inline ModInt query(const unsigned &L, const unsigned &R, int u=1, unsigned l=1, unsigned r=n) {
	debug("query: [%d,%d] %d %d %d\n", L, R, u, l, r);
	if (L <= l && r <= R)
		return tr[u].sum;
	unsigned mid = l + r >> 1;
	pushdown(u, mid - l + 1, r - mid);
	ModInt ret(0);
	if (L <= mid) ret += query(L, R, u<<1, l, mid);
	if (R > mid) ret += query(L, R, u<<1|1, mid+1, r);
	return ret;
}
using std::swap;
inline void add(int x, int y, const int &z) {
	ModInt Z(z);
	while (a[x].top != a[y].top) {
		if (a[a[x].top].dep < a[a[y].top].dep) swap(x, y);
		upd(Z, a[a[x].top].dfn, a[x].dfn);
		x = a[a[x].top].fa;
	}
	if (a[x].dfn > a[y].dfn) swap(x, y);
	upd(z, a[x].dfn, a[y].dfn);
}
inline int sum(int x, int y) {
	ModInt ret;
	while (a[x].top ^ a[y].top) {
		if (a[a[x].top].dep < a[a[y].top].dep) swap(x, y);
		ret += query(a[a[x].top].dfn, a[x].dfn);
		x = a[a[x].top].fa;
	}
	if (x) {
		if (a[x].dfn > a[y].dfn) swap(x, y);
		ret += query(a[x].dfn, a[y].dfn);
	}
	debug("Final Ans: %d\n", ret.num);
	return ret.num;
}
int main() {
#ifndef ONLINE_JUDGE
	freopen("P3384_4.in", "r", stdin);
	freopen("P3384.out", "w", stdout);
	freopen("debug.txt", "w", stderr);
#endif
	scanf("%d%d%d%d", &n, &m, &r, &P);
	for (int i = 1; i <= n; i++)
		scanf("%d", &a[i].val);
	for (int i = 1; i < n; i++) {
		int u, v;
		scanf("%d%d", &u, &v);
		a[u].sons.emplace_back(v);
		a[v].sons.emplace_back(u);
	}
	for (int v : a[r].sons) debug("%d ", v); debug("\n");
	dfs1(r, 0);
	a[r].top = r; dfs2(r, 0);
	for (int i = 1; i <= n; i++)
		upd(ModInt(a[i].val), a[i].dfn, a[i].dfn);
	debug("-------------Options started.----------------\n");
	for (int opt, x, y, z; m--; ) {
		scanf("%d%d", &opt, &x);
		switch (opt) {
		case 1:
			scanf("%d%d", &y, &z);
			add(x, y, z);
			break;
		case 2:
			scanf("%d", &y);
			printf("%d\n", sum(x, y));
			break;
		case 3:
			scanf("%d", &z);
			upd(ModInt(z), a[x].dfn, a[x].edfn);
			break;
		case 4:
			printf("%d\n", query(a[x].dfn, a[x].edfn).num);
		}
	}
	return 0;
}
