#include <bits/stdc++.h>
using namespace std;
constexpr int N = 200005;
using Range = pair<int,int>;
Range a[N], b[N];
struct Hash {
    uint64_t operator()(const Range &x) const {
        return (uint64_t)x.first << 32 | x.second;
    }
};
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        long long ans = 0;
        for (int i = 1; i <= n; i++) {
            scanf("%d%d", &a[i].first, &a[i].second);
            b[i] = a[i];
            ans += a[i].second - a[i].first;
        }
        sort(a+1, a+1 + n, [](Range&x, Range&y){return x.first+x.second < y.first+y.second;});
        int mn = 1 << 30, mx = 0;
        long long lsum(0), rsum(0);
        for (int i = 1; (i<<1) <= n; i++) {
            lsum += a[i].first;
            mx = max(mx, a[i].first);
            rsum += a[n-i+1].second;
            mn = min(mn, a[n-i+1].second);
        }
        if (n & 1) {
            auto [p, q] = a[n+1 >> 1];
            ans += max(rsum - (lsum + p - max(mx, p)), (rsum + q - min(mn, q)) - lsum);
        } else ans += rsum - lsum;
        printf("%lld\n", ans);
    }
    return 0;
}