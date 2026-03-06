#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 1000005;
char s[N];
int sa[N], rk[N], height[N];
inline void getSA(int n, char *s, int *sa, int *rk_, int *height, int sigma = 127) {
    static int cnt[N], vec[N], tpsa[N];
    int *rk = rk_, *tprk = vec;
    auto radixSort = [&]() {
        memset(cnt, 0, sizeof(int) * (sigma + 1));
        for (int i = 1; i <= n; i++) ++cnt[rk[i]];
        for (int i = 1; i <= sigma; i++) cnt[i] += cnt[i-1];
        for (int i = n; i >= 1; i--) sa[cnt[rk[tpsa[i]]]--] = tpsa[i];
    };
    for (int i = 1; i <= n; i++) tpsa[i] = i, rk[i] = s[i];
    radixSort();
    for (int w = 1; w < n; w <<= 1) {
        int idx = 0;
        for (int i = n - w + 1; i <= n; i++) tpsa[++idx] = i;
        for (int i = 1; i <= n; i++)
            if (sa[i] > w) tpsa[++idx] = sa[i] - w;
        radixSort();
        sigma = 0;
        for (int i = 1; i <= n; i++)
            tprk[sa[i]] = sigma += !(rk[sa[i]] == rk[sa[i-1]] && rk[sa[i]+w] == rk[sa[i-1]+w]);
        std::swap(rk, tprk);
        if (sigma == n) break;
    }
    if (rk != rk_) memcpy(rk_, rk, sizeof(int) * (n + 1));
    for (int i = 1, k = 0; i <= n; i++) {
        if (rk[i] == 1) continue;
        if (k) --k;
        while (i+k<=n && sa[rk[i]-1]+k<=n && s[i+k] == s[sa[rk[i]-1]+k]) ++k;
        height[rk[i]] = k;
    }
}
int main() {
    scanf("%s", s+1);
    int n = strlen(s+1);
    getSA(n, s, sa, rk, height);
    for (int i = 1; i <= n; i++)
        printf("%d ", sa[i]);
    putchar('\n');
    return 0;
}