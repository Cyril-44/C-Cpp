#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
constexpr int N = 200005;

struct Num {
	int x, id;
	inline bool operator< (const Num &rhs) const {
		return x > rhs.x || x == rhs.x && id < rhs.id;
	}
} b[N];
int a[N];
i64 sum[N];
namespace STVec {
    i64 mn[20][N], mx[20][N];
    void init(const int n) {
        for (int i = 1; i <= n; i++)
            mn[0][i] = mx[0][i] = a[i];
        for (int k = 1; k <= 18; ++k)
            for (int i = 1, j = 1 + (1 << k-1); i + (1 << k) - 1 <= n; ++i, ++j)
                mn[k][i] = min(mn[k-1][i], sum[j-1] - sum[i-1] + mn[k-1][j]),
                mx[k][i] = max(mx[k-1][i], sum[j-1] - sum[i-1] + mx[k-1][j]);
    }
    inline i64 premn(const int l, const int r) {
        if (l > r) return 0;
        int k = 31 - __builtin_clz(r - l + 1);
        int mid = r - (1 << k) + 1;
        return min(mn[k][l], sum[mid-1] - sum[l-1] + mn[k][mid]);
    }
    inline i64 premx(const int l, const int r) {
        if (l > r) return 0;
        int k = 31 - __builtin_clz(r - l + 1);
        int mid = r - (1 << k) + 1;
        return max(mx[k][l], sum[mid-1] - sum[l-1] + mx[k][mid]);
    }
};
using STVec::premn;
using STVec::premx;
int main() {
	int n, k;
	scanf("%d%d", &n, &k);
	for (int i = 1; i <= n; i++) {
		scanf("%d", &a[i]);
		b[i] = {a[i], i};
        sum[i] = sum[i-1] + a[i];
	}
    if (!k) { // k=0 特判
        i64 sum = 0, ans = numeric_limits<i64>::min();
        for (int i = 1; i <= n; i++) {
            if (sum < 0) sum = 0;
            sum += a[i];
            ans = max(ans, sum);
        }
        printf("%lld\n", ans);
        return 0;
    }
    STVec::init(n);
	sort(b+1, b+1 + n); // gt
    list<int> gt({0, n+1});  // Guard
    using Key = pair<int, list<int>::iterator>;
    struct KeyComp { __attribute__((always_inline)) inline bool operator()(const Key &x, const Key &y) const { return x.first < y.first; } };
    set<Key, KeyComp> st{{0, gt.begin()}, {n+1, --gt.end()}};

    i64 ans = 0;
    for (int i = 1; i <= n; i++) { // 枚举 b[i] (ai 从大到小)
        // if (end < n && b[end].x == b[end+1].x) continue;
        auto pos = gt.insert(st.lower_bound(Key(b[i].id, list<int>::iterator())) -> second, b[i].id);
        st.emplace(b[i].id, pos);
        if (i >= k) {
            auto l = pos, r = pos; // (l, r]
            int front = 0, back = 0;
            i64 mxks = 0;
            while (front < k) {
                mxks += a[*l]; ++front;
                if (--l == gt.begin()) break;
            }
            for (back = k - front; back; --back)
                mxks += a[*(++r)];
            for (; r != --gt.end() && l != pos; mxks += -a[*(++l)] + a[*(++r)]) {
                ans = max(ans, sum[*r] - sum[*l] - mxks - min(0ll, premn(*l + 1, *next(l) - 1)) + max(0ll, premx(*r+1, *next(r) - 1)));
                // printf("(%d, %d]: (-)%lld (+)%lld [%d,%d](-)%lld [%d,%d](+)%lld\n", 
                //     *l, *r, mxks, sum[*r] - sum[*l], *l+1, *next(l)-1, premn(*l + 1, *next(l) - 1), *r+1, *next(r)-1, premx(*r + 1, *next(r) - 1));
            }
        }
    }
    printf("%lld\n", ans);
	return 0;
}
