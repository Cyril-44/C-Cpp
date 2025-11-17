#include <stdio.h>
#include <string.h>
#include <algorithm>
constexpr int N = 205, M = 100005;
int a[N], b[N], amount[N], c[N], d[N];
long long f[2][M * 2], g[M * 2];
int l[N], r[N];
int gp[M * 2];
int main() {
	int n;
	scanf("%d", &n);
	int sum = 0, cnt = 0;
    for (int i = 1; i <= n; i++) {
		scanf("%d%d%d%d", a+i, b+i, c+i, d+i);
        sum += a[i] * d[i];
        cnt += a[i] * c[i];
        amount[i] = b[i] - a[i];
    }
	memset(f, -0x3f, sizeof(f) >> 1);
	f[0][0+M] = 0;
    auto *now = f[1], *pre = f[0];
	for (int i = 1; i <= n; i++) {
        for (int j = 0; j < c[i]; j++)
            l[j] = j, r[j] = j - c[i];
        memset(g, -0x3f, sizeof g);
        int mod = 0, div = 0;
        for (int j = 0; j < 2*M; j++) {
            long long tmp = pre[j] - 1ll * d[i] * div;
            while (l[mod] <= r[mod] && div - gp[l[mod]] > amount[i]) l[mod] += c[i];
            while (l[mod] <= r[mod] && g[r[mod]] <= tmp) r[mod] -= c[i];
            g[r[mod] += c[i]] = tmp; gp[r[mod]] = div;
            now[j] = g[l[mod]] + 1ll * d[i] * div;
            // if (now[j] >= -1000000) printf("%d %d: %d %d %lld %lld %lld\n", i, j, gp[l[mod]], gp[r[mod]], g[l[mod]], g[r[mod]], now[j]);
            if (++mod == c[i]) mod = 0, ++div;
        }
        std::swap(now, pre);
	}
	printf("%lld\n", sum + pre[M - cnt]);
	return 0;
}