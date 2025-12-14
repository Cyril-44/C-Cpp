#include <cstdio>
#include <cstring>
#include <algorithm>
#define debug(...) 0//fprintf(stderr, __VA_ARGS__)
using std::min;
using std::max;
const int N = 100005;
struct Line {
    int x1, x2, y, f;
    inline bool operator<(const Line &x) const {
    	return y < x.y || y == x.y && f > x.f;
	}
} a[N << 1];
namespace Unique {
    int a[N<<1];
    int tot(0);
    inline void add(const int &x) { a[++tot] = x; }
    inline void build() { std::sort(a+1, a+1 + tot); tot = std::unique(a+1, a+1 + tot) - a-1; }
    inline int p(const int &x) { return std::lower_bound(a+1, a+1 + tot, x) - a; }
} using Unique::p;
namespace SegTree {
    struct SegNode {
        int cnt, len;
    } tr[N << 2];
    inline void pushup(const int &u, const int &l, const int &r) {
        if (tr[u].cnt) tr[u].len = Unique::a[r] - Unique::a[l];
        else if (l == r) tr[u].len = 0;
        else tr[u].len = tr[u<<1].len + tr[u<<1|1].len;
    }
    inline void upd(const int &x, const int &L, const int &R, const int &u=1, const int &l=1, const int &r=Unique::tot) {
        debug("%d %d %d %d %d\n", L, R, u, l, r);
		if (L == l && r == R) { tr[u].cnt += x; pushup(u, l, r); return; }
        int mid = l + r >> 1;
        if (L < mid) upd(x, L, min(R, mid), u<<1, l, mid);
        if (mid < R) upd(x, max(mid, L), R, u<<1|1, mid, r);
        pushup(u, l, r);
    }
}
int main() {
int n;
    scanf("%d", &n);
    for (int i=0, x1, y1, x2, y2; i < n; i++) {
        scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
        Unique::add(x1), Unique::add(x2);
        a[i<<1] = Line{x1, x2, y1, 1};
        a[i<<1|1] = Line{x1, x2, y2, -1};
    }
    Unique::build();
    std::sort(a, a + (n<<1));
    SegTree::upd(a->f, p(a->x1), p(a->x2));
    long long ans = 0;
	for (int i = 1; i < (n<<1); i++) {
		ans += (long long)(a[i].y - a[i-1].y) * SegTree::tr[1].len;
		SegTree::upd(a[i].f, p(a[i].x1), p(a[i].x2));
	}
	printf("%lld\n", ans);
    return 0;
}