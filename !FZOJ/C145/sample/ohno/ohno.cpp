#include <stdio.h>
#include <string.h>
#include <algorithm>
constexpr int N = 10000005;
using int8 = signed char;
int8 a[N]; // -1, 0, 1
int cnt[N], sum[N], sum1[N], q[N], sum_1[N];
namespace FastI {
    char buf[1 << 21], *p1=nullptr, *p2=nullptr;
    inline char get() {
        if (p1 == p2) p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++;
    }
    inline void in(int &x) {
        char ch = get();
        while (ch < '0' || ch > '9') ch = get();
        for (x = ch ^ '0', ch = get(); ch >= '0' && ch <= '9'; ch = get())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
    inline void in(int8 &x) {
        char ch = get();
        while (true) {
            switch (ch) {
            case '-': ch = get(); x = -1; return;
            case '1': x = 1;
            case '0': return;
            default : ch = get();
            }
        }
    }
} using FastI::in;
inline void bruteForce(int n, int k) {
    k <<= 1;
    int ans = -1;
    for (int l = 0; l < n; l++)
        for (int r = l+1; r <= n; r++) {
            int zeros = cnt[r] - cnt[l];
            ans = std::max(ans, sum[r] - sum[l] + std::min(zeros, k - zeros));
        }
    printf("%d\n", ans);
}
int main() {
    freopen("ohno.in", "r", stdin);
    freopen("ohno.out", "w", stdout);
    int n, k;
    in(n), in(k);
    for (int i = 1; i <= n; i++) {
        in(a[i]);
        sum[i] = sum[i-1] + a[i];
        cnt[i] = cnt[i-1] + !a[i];
        sum1[i] = sum1[i-1] + (a[i] ? a[i] : 1);
        sum_1[i] = sum_1[i-1] + (a[i] ? a[i] : -1);
    }
    if (n <= 5000) return bruteForce(n, k), 0;
    int ans = 0, mn_1 = n;
    int l = 0, r = 0, L = 0; // a[q[i]] 单调递增，取 q[l] 为最小的 sum1 位置
    for (int i = 0; i <= n; i++) {
        while (cnt[L] + k < cnt[i]) mn_1 = std::min(mn_1, sum_1[L++]);
        while (l < r && cnt[q[l]] + k < cnt[i]) ++l;
        while (l < r && sum1[q[r-1]] >= sum1[i]) --r;
        q[r++] = i;
        ans = std::max(ans, sum1[i] - sum1[q[l]]);
        if (cnt[i] >= k)
            ans = std::max(ans, sum_1[i] + k * 2 - mn_1);
    }
    printf("%d\n", ans);
    return 0;
}