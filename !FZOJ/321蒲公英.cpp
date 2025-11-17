#include <stdio.h>
#include <string.h>
#include <math.h>
#include <algorithm>
#ifdef ONLINE_JUDGE
# undef fprintf
# define fprintf(...) ((void)0)
#endif
constexpr int N = 50005, B = 305;
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
		return _[mxp];
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
		if (n % bsz) memcpy(sum[++bcnt], h, _tofill);
		for (int i = 1; i <= bcnt; i++) {
			memset(h, 0, _tofill);
			int l = (i-1) * bsz + 1;
			int mx = 0, mxp = -1;
			for (int r = l; r <= n; ++r) {
				if (++h[a[r]] > mx) mx = h[a[r]], mxp = a[r];
				else if (h[a[r]] == mx && a[r] < mxp) mxp = a[r]; // 一定要注意 取最小。。。
				if (r % bsz == 0) com[i][r / bsz] = mxp;
			}
			com[i][bcnt] = mxp;
		}
	}
	inline int query(const int l, const int r) {
		static int h[N];
		static bool vis[N];
		int bl = (l + bsz - 1) / bsz + 1, br = r / bsz;
		memset(h, 0, _tofill);
		memset(vis, 0, _tofill);
		if (br - bl <= 1) {
			int mx = 0, mxp = -1;
			for (int i = l; i <= r; i++)
				if (++h[a[i]] > mx) mx = h[a[i]], mxp = a[i];
				else if (h[a[i]] == mx && a[i] < mxp) mxp = a[i];
			return _[mxp];
		}
		int mxp = com[bl][br], mx = sum[br][mxp] - sum[bl - 1][mxp];
		h[mxp] = mx;
		vis[mxp] = true;
		fprintf(stderr, "%d, %d (%d [%d,%d] %d) ", _[mxp], mx, l, (bl-1) * bsz + 1, br * bsz, r);
        for (int st : {0, 1}) {
			for (int i = st ? br * bsz + 1 : l; i <= (st ? r : (bl-1) * bsz); ++i) {
				++h[a[i]];
				if (!vis[a[i]]) {
					vis[a[i]] = true;
					h[a[i]] += sum[br][a[i]] - sum[bl-1][a[i]];
				}
				if (h[a[i]] > mx || h[a[i]] == mx && a[i] < mxp) mx = h[a[i]], mxp = a[i];
				if (_[a[i]]==404964756) fprintf(stderr, "!%d! ", h[a[i]]);
			}
		}
		fprintf(stderr, "%d %d\n", _[mxp], mx);
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