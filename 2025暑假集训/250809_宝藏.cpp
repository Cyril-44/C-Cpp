#include <cstdio>
#include <cstring>
#include <algorithm>
#if !defined(ONLINE_JUDGE)
#include <assert.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug(...) 0
#endif
typedef long long i64;
constexpr int N = 300005;
int n;
template<typename T>
struct BIT {
	T tr[N];
	inline BIT () { memset(tr, 0, sizeof tr); }
	inline void upd(int x, const T &y) {
		if (!x) return;
		while (x <= n) tr[x] += y, x += x & -x;
	}
	inline T sum(int x) const {
		T res = 0;
		while (x) res += tr[x], x -= x & -x;
		return res;
	}
};
BIT<int> cntL, cntR;
BIT<i64> sumL, sumR;
inline i64 getsum(const int &x, const BIT<int> &cnt, const BIT<i64> &sum) {
	int l = 0, r = n, mid;
	if (cnt.sum(r) < x) return (i64)(1e18);
	while (l <= r) {
		mid = l + (r - l >> 1);
		// debug("mid:%d res:%d\n", mid, cnt.sum(mid));
		if (cnt.sum(mid) < x) l = mid + 1;
		else r = mid - 1;
	}
	return sum.sum(l);
}
struct Node {
	int w, t, id;
} a[N];
inline void init(Node *arr, const int &n) {
	std::sort(arr+1, arr+1 + n, [](const Node&x, const Node&y){ return x.t < y.t; });
	for (int i = 1; i <= n; i++) arr[i].id = i;
	std::sort(arr+1, arr+1 + n, [](const Node&x, const Node&y){ return x.w < y.w; });
}
int ans[N];
int main() {
	int q;
	long long t;
	scanf("%d%lld%d", &n, &t, &q);
	for (int i = 1; i <= n; i++)
		scanf("%d%d", &a[i].w, &a[i].t);
	init(a, n);
	for (int i = 1; i <= n; i++) {
		cntL.upd(a[i].id, 1);
		sumL.upd(a[i].id, a[i].t);
	}
	int now = 1;
	memset(ans, -1, sizeof ans);
	for (int i = n; i >= 1; i--) {
		cntL.upd(a[i].id, -1);
		sumL.upd(a[i].id, -a[i].t);
		while (now <= ((n-i)<<1|1) && now <= n && getsum(now>>1, cntL, sumL) + getsum(now>>1, cntR, sumR) + a[i].t <= t)
			ans[now] = a[i].w, now += 2;
		// debug("Loop %d: now:%d t:%d L:%lld R:%lld\n", i, now, a[i].t, getsum(now-1>>1, cntL, sumL), getsum(now-1>>1, cntR, sumR), getsum(now+1>>1, cntL, sumL), getsum(now+1>>1, cntR, sumR));
		cntR.upd(a[i].id, 1);
		sumR.upd(a[i].id, a[i].t);
	}
	for (int x; q--; ) {
		scanf("%d", &x);
		printf("%d\n", ans[x]);
	}
	return 0;
}