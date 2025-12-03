#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <bits/stdc++.h>
using namespace std;
#define umax(x, y) ((x) = std::max((x), (y)))
constexpr int N = 500005, NINF = -(1 << 30);
#define Inline __attribute__((always_inline)) inline
int n;
namespace SegTr {
struct Node {
	int size; // 这个节点代表的区间的长度
	int sum, mx, mx2, mxcnt, hmx;
// 懒标记:
	struct Tag {
		int mx, hmx; // 对于 mx 的懒标记，从该节点 pushdown 以来的最大 mx 懒标记
		int x, hx; // 对于不是 mx 的懒标记，从该节点 pushdown 以来的最大 非mx 懒标记
	} add;
	inline void pull(const Tag &t) { // 先更新历史 再更新当前
	// 更新线段树
		umax(hmx, mx + t.hmx);
		sum += mxcnt * t.mx + (size - mxcnt) * t.x;
		mx += t.mx;
		if (mx2 ^ NINF) mx2 += t.x;
	// 更新树上懒标记
		umax(add.hmx, add.mx + t.hmx);
		umax(add.hx, add.x + t.hx);
		add.mx += t.mx, add.x += t.x
		;
	}
} tr[N << 2];
void pushdown(int u) {
	if (tr[u<<1].mx == tr[u].mx)
		tr[u<<1].pull(tr[u].add);
	else tr[u<<1].pull({tr[u].add.x, tr[u].add.hx, tr[u].add.x, tr[u].add.hx});
	if (tr[u<<1|1].mx == tr[u].mx)
		tr[u<<1|1].pull(tr[u].add);
	else tr[u<<1|1].pull({tr[u].add.x, tr[u].add.hx, tr[u].add.x, tr[u].add.hx});
	memset(&tr[u].add, 0, sizeof tr[u].add);
}
inline void pushup(int u) {
	Node& us = tr[u];
	const Node& ls = tr[u<<1], &rs = tr[u<<1|1];
	us.mx = std::max(ls.mx, rs.mx);
	us.hmx = std::max(ls.hmx, rs.hmx);
	us.sum = ls.sum + rs.sum;
	if (ls.mx == rs.mx) {
		us.mxcnt = ls.mxcnt + rs.mxcnt;
		us.mx2 = std::max(ls.mx2, rs.mx2);
	} else if (ls.mx == us.mx) {
		us.mxcnt = ls.mxcnt;
		us.mx2 = std::max(ls.mx2, rs.mx);
	} else {
		us.mxcnt = rs.mxcnt;
		us.mx2 = std::max(rs.mx2, ls.mx);
	}
}
void build(int u = 1, int l = 1, int r = n) {
	if (l == r) {
		scanf("%d", &tr[u].mx);
		tr[u].sum = tr[u].hmx = tr[u].mx;
		tr[u].mx2 = NINF;
		tr[u].mxcnt = tr[u].size = 1;
		return;
	}
	int mid = l + r >> 1;
	build(u<<1, l, mid);
	build(u<<1|1, mid+1, r);
	tr[u].size = r - l + 1;
	pushup(u);
}
int L, R, X;
#define QUE(var, func, deft) \
	int que##var(int u, int l, int r) { \
		if (r < L || R < l) return deft; \
		if (L <= l && r <= R) return tr[u].var; \
		int mid = l + r >> 1; \
		pushdown(u); \
		return func(que##var(u<<1, l, mid), que##var(u<<1|1, mid+1, r)); \
	} \
	inline int query_##var(int l, int r) { \
		L = l, R = r; \
		return que##var(1, 1, n); \
	}
#define add(x, y) ((x) + (y))
QUE(mx, std::max, NINF)
QUE(hmx, std::max, NINF)
QUE(sum, add, 0)
void updmn(int u, int l, int r) {
	if (r < L || R < l) return;
	if (L <= l && r <= R) {
		if (tr[u].mx <= X) return; // 没有任何影响
		if (tr[u].mx2 < X && X < tr[u].mx) { // second_max < modify < max
			tr[u].sum -= (tr[u].mx - X) * tr[u].mxcnt; // 变为对于 max 加上 (modify - max)
			tr[u].add.mx += X - tr[u].mx;
			tr[u].mx = X;
			return;
		}
	}
	int mid = l + r >> 1;
	pushdown(u);
	updmn(u<<1, l, mid);
	updmn(u<<1|1, mid+1, r);
	pushup(u);
}
void updadd(int u, int l, int r) {
	if (r < L || R < l) return;
	if (L <= l && r <= R) {
		tr[u].pull({X, X, X, X});
		return;
	}
	int mid = l + r >> 1;
	pushdown(u);
	updadd(u << 1, l, mid);
	updadd(u << 1 | 1, mid+1, r);
	pushup(u);
}
inline void updatemin(int l, int r, int x) {
	L = l, R = r, X = x;
	updmn(1, 1, n);
}
inline void updateadd(int l, int r, int x) {
	L = l, R = r, X = x;
	updadd(1, 1, n);
}
#undef add
#undef QUE
}
int main() {
	int m;
	scanf("%d%d", &n, &m);
	SegTr::build();
	for (int op, l, r, v; m--; ) {
		scanf("%d%d%d", &op, &l, &r);
		if (op <= 2) scanf("%d", &v);
		switch (op) {
		case 1: SegTr::updateadd(l, r, v); break;
		case 2: SegTr::updatemin(l, r, v); break;
		case 3: printf("%d\n", SegTr::query_sum(l, r)); break;
		case 4: printf("%d\n", SegTr::query_mx(l, r)); break;
		case 5: printf("%d\n", SegTr::query_hmx(l, r)); break;
		}
	}
	return 0;
}
