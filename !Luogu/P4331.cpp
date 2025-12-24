#include <bits/extc++.h>
#include <numeric>
using namespace __gnu_pbds;
constexpr int N = 1000005;
using LL = long long;
int a[N], cnt[N];
priority_queue<int> pq[N];
LL sum[N], sdrop[N];
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
		ans += sdrop[i] - pq[i].top() * (long long)(cnt[i] - pq[i].size()) + pq[i].top() * (long long)pq[i].size() - (sum[i] - sdrop[i]);
    }
    printf("%lld\n", ans);
	for (int i = 1, j = 1; i <= top; i++) {
		for (; cnt[i]--; j++) printf("%d\n", pq[i].top() + j);
	}
	return 0;
}