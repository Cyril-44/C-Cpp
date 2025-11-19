#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <assert.h>
constexpr int N = 30005;
using LL = long long;
#ifdef DEBUG
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug(...) ((void)0)
#endif
struct Fact {
	LL x, y;
	inline Fact(LL _x, LL _y) {
		LL tp = std::__gcd(_x, _y);
		x = _x / tp, y = _y / tp;
	}
	inline bool operator< (const Fact &rhs) const { // x/y < r.x/r.y
		return x * rhs.y < y * rhs.x;
	}
	inline bool operator> (const Fact &rhs) const {
		return rhs < *this;
	}
	inline Fact operator- (const Fact &rhs) const { // x/y - r.x/r.y = x*r.y - y*r.x / y*r.y
		return Fact(x * rhs.y - y * rhs.x, y * rhs.y);
	}
	inline Fact operator* (const Fact &rhs) const {
		return Fact(x * rhs.x, y * rhs.y);
	}
};
int main() {
	int p, n;
	scanf("%d%d", &p, &n);
	Fact mnl(n*n,1), mnr(n*n,1), xl(1,1), xr(1,1);
	Fact P(p, 1);
	for (int i = 1; i <= n; i++) {
		int l = 1, r = n, mid;
		while (l <= r) {
			mid = l + r >> 1;
			Fact tmp(mid, i);
			tmp = tmp * tmp;
			if (tmp < P) l = mid + 1;
			else r = mid - 1;
		}
		if (l == n+1) l = n;
		debug("%d/%d %d/%d ", l, i, r, i);
		Fact tmp(r, i); tmp = tmp * tmp;
		if (tmp < P) {
			Fact now = P - tmp;
			if (now < mnl) mnl = now, xl = Fact(r, i);
			debug("%lld/%lld %lld/%lld, ", tmp.x, tmp.y, now.x, now.y);
		}
		tmp = Fact(l, i); tmp = tmp * tmp;
		if (tmp > P) {
			Fact now = tmp - P;
			if (now < mnr) mnr = now, xr = Fact(l, i);
			debug("%lld/%lld %lld/%lld, ", tmp.x, tmp.y, now.x, now.y);
		}
        debug("\n");
	}
	printf("%lld/%lld %lld/%lld\n", xl.x, xl.y, xr.x, xr.y);
	return 0;
}