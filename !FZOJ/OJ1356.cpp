#include <bits/extc++.h>
using namespace __gnu_pbds;
constexpr int N = 1000005;
int a[N], cnt[N];
priority_queue<int> pq[N];
long long sum[N], sdrop[N];
int main() {
	int n;
	scanf("%d", &n);
	for (int i = 1; i <= n; i++)
		scanf("%d", &a[i]);
	int top = 0;
	long long ans = 0;
	for (int i = 1; i <= n; i++) {
		pq[++top].clear();
		cnt[top] = 1;
		sdrop[top] = 0, sum[top] = a[i];
		pq[top].push(a[i]);
		while (top > 1 && pq[top].top() < pq[top-1].top()) {
			pq[top-1].join(pq[top]);
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
	return 0;
}