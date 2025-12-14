#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
constexpr int N = 100005;
typedef long long LL;
int n, P, a[N];
struct SegNode {
	LL sum, add, mul;
	inline SegNode() : sum(), add(), mul(1ll) {}
} tr[N << 3];
inline void pushup(const int &u) {
	tr[u].sum = (tr[u<<1].sum + tr[u<<1|1].sum) % P;
}
inline void pushdown(const int &u, const int &ls, const int &rs) {
	if (tr[u].mul ^ 1) {
		tr[u<<1].mul = tr[u<<1].mul * tr[u].mul % P;
		tr[u<<1|1].mul = tr[u<<1|1].mul * tr[u].mul % P;
		tr[u<<1].add = tr[u<<1].add * tr[u].mul % P;
		tr[u<<1|1].add = tr[u<<1|1].add * tr[u].mul % P;
		tr[u<<1].sum = tr[u<<1].sum * tr[u].mul % P;
		tr[u<<1|1].sum = tr[u<<1|1].sum * tr[u].mul % P;
		tr[u].mul = 1 % P;
	}
	if (tr[u].add) {
		tr[u<<1].add = (tr[u<<1].add + tr[u].add) % P;
		tr[u<<1|1].add = (tr[u<<1|1].add + tr[u].add) % P;
		tr[u<<1].sum = (tr[u<<1].sum + tr[u].add * ls) % P;
		tr[u<<1|1].sum = (tr[u<<1|1].sum + tr[u].add * rs) % P;
		tr[u].add = 0ll;
	}
}
void build(int u=1, int l=1, int r=n) {
	if (l == r) return tr[u].sum = a[l] % P, void();
	int mid = l + r >> 1;
	build(u << 1, l, mid);
	build(u << 1 | 1, mid + 1, r); 
	pushup(u);
}
const auto add = [](const int &u, const int &x, const int &len) -> void {
	tr[u].sum = (tr[u].sum + (LL)x * len % P) % P;
	tr[u].add = (tr[u].add + x) % P;
};
const auto mul = [](const int &u, const int &x, const int &len) -> void {
	tr[u].sum = tr[u].sum * x % P;
	tr[u].add = tr[u].add * x % P;
	tr[u].mul = tr[u].mul * x % P;
};
void upd(void (*func)(const int&,const int&,const int&), const int &x, const int &L, const int &R, const int &u=1, const int &l=1, const int &r=n) {
	if (L <= l && r <= R) return func(u, x, r - l + 1);
	int mid = l + r >> 1;
	pushdown(u, mid - l + 1, r - mid);
	if (L <= mid) upd(func, x, L, R, u<<1, l, mid);
	if (mid < R) upd(func, x, L, R, u<<1|1, mid+1, r);
	pushup(u);
}
inline LL query(const int &L, const int &R, const int &u=1, const int &l=1, const int &r=n) {
	if (L <= l && r <= R) return tr[u].sum;
	int mid = l + r >> 1;
	pushdown(u, mid - l + 1, r - mid);
	LL ret = 0;
	if (L <= mid) ret = query(L, R, u<<1, l, mid);
	if (mid < R) ret = (ret + query(L, R, u<<1|1, mid+1, r)) % P;
	return ret;
}
int main() {
#ifndef ONLINE_JUDGE
	freopen("P3373_2.in", "r", stdin);
	freopen("res.out", "w", stdout);
#endif
	int m;
	scanf("%d%d", &n, &P);
	for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
	build();
	scanf("%d", &m);
	for (int ops, t, g, c; m--; ) {
		scanf("%d%d%d", &ops, &t, &g);
		switch (ops) {
		case 1: scanf("%d", &c);
			upd(mul, c % P, t, g);
			break;
		case 2: scanf("%d", &c);
			upd(add, c % P, t, g);
			break;
		case 3:
			printf("%lld\n", query(t, g));
		}
	}
	return 0;
}


