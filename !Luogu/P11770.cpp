#include <bits/stdc++.h>
using namespace std;
constexpr int N = 2000005;
using i64 = long long;
i64 k[N], b[N];
int p[N >> 2], mxfac[N], cnt[N];
bool np[N];
auto __init__ = [](const int n = 2000000) {
    for (int i = 2; i <= n; i++) {
        if (!np[i]) p[++p[0]] = i, mxfac[i] = i;
        for (int j = 1, tp; j <= p[0] && (tp = i * p[j]) <= n; j++) {
            np[tp] = true;
            mxfac[tp] = mxfac[i];
            if (i % p[j] == 0) break;
        }
    }
    for (int i = 2; i <= n; i++) {
        cnt[i / mxfac[i]]++;
    }
    // 计算 k[j] = sum_{i | j} cnt[i] ，通过遍历每个 d 的倍数
    for (int i = 1; i <= n; i++) {
        for (int j = i; j <= n; j += i)
            k[j] += cnt[i];
    }
    for (int i = 1; i <= n; i++) k[i] += k[i-1]; // 前缀和
    // b[1] = 1;
    for (int i = 2; i <= n; i++) {
        for (int j = i / mxfac[i]; j > 1; j /= mxfac[j])
            b[i] += 1 - j, k[i] += k[j];
    }
    for (int i = 1; i <= n; i++) k[i] += k[i-1];
    for (int i = 1; i <= n; i++) b[i] += b[i-1]; // 前缀和
    return nullptr;
}();
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        if (n == 1) { puts("1"); continue; }
        printf("%lld\n", k[n] + b[n]);
    }
    return 0;
}