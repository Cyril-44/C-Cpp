#include <bits/extc++.h>
#include <numeric>
using namespace __gnu_pbds;
constexpr int N = 1000005;
using LL = long long;
int a[N], cnt[N];
priority_queue<int> pq[N];
LL sum[N], sdrop[N];
#undef assert
#define assert(expr) do {if (!(expr)) throw;} while (0)
signed main() {
	int n;
	scanf("%d", &n);
	for (int i = 1; i <= n; i++) {
		scanf("%d", &a[i]);
		a[i] -= i;
	}
	int top = 0;
	LL ans = 0;
	for (int i = 1; i <= n; i++) {
		++top;
		cnt[top] = 1;
		sdrop[top] = 0, sum[top] = a[i];
		pq[top].push(a[i]);
		while (top > 1 && pq[top].top() <= pq[top-1].top()) {
			pq[top-1].join(pq[top]);
			pq[top].clear();
			sum[top-1] += sum[top];
            sdrop[top-1] += sdrop[top];
            cnt[top-1] += cnt[top];
			--top;
			while ((int)pq[top].size() > (cnt[top] + 1 >> 1))
				sdrop[top] += pq[top].top(), pq[top].pop();
		}
	}
    for (int i = 1; i <= top; i++) {
		// assert(sdrop[i] - pq[i].top() * (long long)(cnt[i] - pq[i].size()) >= 0);
		// assert(pq[i].top() * (long long)pq[i].size() - (sum[i] - sdrop[i]) >= 0);
		// assert(pq[i].size() >= cnt[i] / 2 && pq[i].size() <= (cnt[i] + 1) / 2);
		// assert(i == 1 || pq[i].top() > pq[i-1].top());
		// if (!(std::accumulate(pq[i].begin(), pq[i].end(), 0ll) == sum[i] - sdrop[i])) throw;
		// fprintf(stderr, "%d %d %d %lld %lld +%lld\n", cnt[i], pq[i].top(), (int)pq[i].size(), sum[i], sdrop[i], sdrop[i] - pq[i].top() * (long long)(cnt[i] - pq[i].size()) + pq[i].top() * (long long)pq[i].size() - (sum[i] - sdrop[i]));
        ans += sdrop[i] - pq[i].top() * (long long)(cnt[i] - pq[i].size()) + pq[i].top() * (long long)pq[i].size() - (sum[i] - sdrop[i]);
    }
    printf("%lld\n", ans);
	// for (int i = 1, j = 1; i <= top; i++) {
	// 	for (; cnt[i]--; j++) printf("%d\n", pq[i].top() + j);
	// }
	return 0;
}