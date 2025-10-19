#include <stdio.h>
#include <string.h>
#include <cmath>
#include <algorithm>
constexpr int N = 40005, B = 205;
int n;
int a[N], _[N], _tofill;
namespace BF {
	int h[N];
	inline int query(int l, int r) {
		memset(h, 0, _tofill);
		for (int i = l; i <= r; i++)
			++h[a[i]];
		int mx = 0, mxp = -1;
		for (int i = 1; i <= *_; i++)
			if (h[i] > mx) mx = h[i], mxp = i;
		return mxp;
	}
}
namespace BLK {
	int sum[B][N], com[B][B], cnt[B][B], h[N];
	int bsz, bcnt = 0;
	inline void init() {
		bsz = sqrt(n);
		for (int i = 1; i <= n; i++) {
			++h[a[i]];
			if (i % bsz == 0)
				memcpy(sum[++bcnt], h, _tofill);
		}
		memcpy(sum[++bcnt], h, _tofill);
		for (int i = 1; i <= bcnt; i++) {
			memset(h, 0, _tofill);
			int l = (i-1) * bsz + 1;
			int mx = 0, mxp = -1;
			for (int r = l; r <= n; ++r) {
				if (++h[a[r]] > mx) mx = h[a[r]], mxp = a[r];
				if (r % bsz == 0) com[i][r / bsz] = mxp;
			}
			com[i][bcnt] = mxp;
		}
	}
	inline int query(const int l, const int r) {
		static int h[N];
		static bool vis[N];
		int bl = (l + bsz - 1) / bsz, br = r / bsz;
		memset(h, 0, _tofill);
		memset(vis, 0, _tofill/4);
		if (bl == br) {
			int mx = 0, mxp = -1;
			for (int i = l; i <= r; i++)
				if (++h[a[i]] > mx) mx = h[a[i]], mxp = a[i];
			return _[mxp];
		}
		int mxp = com[bl][br], mx = sum[br][mxp] - sum[bl][mxp];
		vis[mxp] = true;
		for (int st : {0, 1}) {
			for (int i = st ? br * bsz + 1 : l; i <= (st ? r : (bl-1) * bsz); ++i) {
				++h[a[i]];
				if (!vis[a[i]]) {
					vis[a[i]] = true;
					h[a[i]] += sum[br][a[i]] - sum[bl-1][a[i]];
				}
				if (h[a[i]] > mx) mx = h[a[i]], mxp = a[i];
			}
		}
		return _[mxp];
	}
}
int main() {
	int m;
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= n; i++)
		scanf("%d", &a[i]), _[i] = a[i];
	std::sort(_+1, _+1+n);
	_tofill = ((*_ = std::unique(_+1, _+1+n) - _) + 1) * sizeof(int);
	for (int i = 1; i <= n; i++)
		a[i] = std::lower_bound(_+1, _+*_, a[i]) - _;
	BLK::init();
	int ans = 0;
	for (int l, r; m--; ) {
		scanf("%d%d", &l, &r);
		l = (l + ans - 1) % n + 1;
		r = (r + ans - 1) % n + 1;
		if (l > r) std::swap(l, r);
		printf("%d\n", ans = BLK::query(l, r));
	}
	return 0;
}
