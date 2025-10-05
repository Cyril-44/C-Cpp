#include <cstdio>
#include <cstring>
#include <queue>
#include <algorithm>
#include <cassert>
#include <cmath>
#if !defined(ONLINE_JUDGE)
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug(...) 0
#endif
using std::min;
struct FastIO {
	static constexpr int B = 1 << 20;
	char ibuf[B], obuf[B+1], *p1, *p2, *p3;
	const char* p4;
	FastIO () : p1(nullptr), p2(nullptr), p3(obuf), p4(obuf + B) {}
	__attribute__((always_inline)) int getchar() {
		return p1 == p2 && (p2 = (p1 = ibuf) + fread(ibuf, 1, B, stdin), p1 == p2) ? EOF : *p1++;
	}
	__attribute__((always_inline)) bool putchar(const char &c) {
		return p3 == p4 && (p3 = obuf, fwrite(obuf, B, 1, stdout) == B) ? false : (*p3++ = c, true);
	}
	__attribute__((always_inline)) void close() {
		fwrite(obuf, p3 - obuf, 1, stdout);
	}
} io;
template<typename T, typename = typename std::enable_if<std::is_integral<T>::value, void>::type>
void in(T &x) {
	char ch = 0;
	while (ch < '0' || ch > '9') assert(~(ch = io.getchar()));
	for (x = 0; ch >= '0' && ch <= '9'; ch = io.getchar())
		x = (x << 3) + (x << 1) + (ch ^ '0');
}
template<typename T, typename = typename std::enable_if<std::is_integral<T>::value, void>::type>
void out(T x) {
	static int sta[32];
	sta[sta[0] = 1] = x % 10;
	while (x /= 10) sta[++sta[0]] = x % 10;
	while (sta[0] > 0) assert(io.putchar(sta[sta[0]--] | '0'));
}
constexpr int N = 105, Q = 15005, M = 100005;
struct Query {
	int l, r, id, blkL, blkR, ans;
	inline bool operator< (const Query &rhs) const {
		return (blkL ^ rhs.blkL) ? l < rhs.l : r < rhs.r;
	}
} qs[Q];
struct Edge {
	int u, v, w;
} e[M];
int g[N][N], _g[N][N];
int prim(const int &n) {
	static int d[N];
	static bool vis[N];
	int res = 0;
	memset(d, 0x3f, sizeof d);
	memset(vis, 0, sizeof vis);
	for (int _ = 1; _ <= n; _++) {
		int u = 0;
		for (int i = 1; i <= n; i++)
			if (!vis[i] && (!u || d[i] < d[u])) u = i;
		if (d[u] ^ 0x3f3f3f3f) res += d[u];
		vis[u] = true;
		for (int v = 1; v <= n; v++) {
			if (u ^ v) d[v] = min(d[v], g[u][v]);
		}
	}
	return res;
}
int main() {
	int n, m, q;
	in(n), in(m), in(q);
	for (int i = 1; i <= m; i++)
		in(e[i].u), in(e[i].v), in(e[i].w);
	int blksz = std::sqrt(m);
	for (int i = 1; i <= q; i++) {
		in(qs[i].l), in(qs[i].r);
		qs[i].id = i;
		qs[i].blkL = qs[i].l / blksz;
		qs[i].blkR = qs[i].r / blksz;
	}
	std::sort(qs+1, qs+1 + q);
	auto add = [](const int &p) {
		const int &u = e[p].u, &v = e[p].v, &w = e[p].w;
		g[u][v] = min(g[u][v], w);
		g[v][u] = min(g[v][u], w);
	};
	for (int i = 1, bl = -1, l, r; i <= q; i++) {
		if (qs[i].blkL != bl) {
			bl = qs[i].blkL;
			r = (bl + 1) * blksz - 1;
			memset(g, 0x3f, sizeof g);
		}
		while (r < qs[i].r) add(++r);
		memcpy(_g, g, sizeof g);
		if (qs[i].blkL == qs[i].blkR) {
			memset(g, 0x3f, sizeof g);
			for (int j = qs[i].l; j <= qs[i].r; j++)
				add(j);
			qs[i].ans = prim(n);
		} else {
			l = (bl + 1) * blksz;
			while (l > qs[i].l) add(--l);
			qs[i].ans = prim(n);
		}
		memcpy(g, _g, sizeof g);
	}
	std::sort(qs+1, qs+1 + q, [](Query&x, Query&y){ return x.id < y.id; });
	for (int i = 1; i <= q; i++)
		out(qs[i].ans), io.putchar('\n');
	io.close();
	return 0;
}