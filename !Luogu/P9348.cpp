#include <string.h>
#include <stdio.h>
#include <algorithm>
constexpr int N = (int)1e6 + 5;
inline void getSA(int n, char *s, int *sa, int *rk_, int *height, int sigma = 127) {
    static int tpsa[N], h[N], cnt[N];
    int *tprk = h, *rk = rk_;
    auto radixSort = [&] () {
        memset(cnt, 0, sizeof(int) * (sigma + 1));
        for (int i = 1; i <= n; i++) ++cnt[rk[i]];
        for (int i = 1; i <= sigma; i++) cnt[i] += cnt[i-1];
        for (int i = n; i >= 1; i--) sa[cnt[rk[tpsa[i]]]--] = tpsa[i];
    };
    for (int i = 1; i <= n; i++) tpsa[i] = 1, rk[i] = s[i];
    radixSort();
    for (int w = 1; w < n; w <<= 1) {
        int top = 0;
        for (int i = n - w + 1; i <= n; i++) tpsa[++top] = i;
        for (int i = 1; i <= n; i++) if (sa[i] > w) tpsa[++top] = sa[i] - w;
        radixSort();
        sigma = 0;
        for (int i = 1; i <= n; i++)
            tprk[sa[i]] = sigma += !(rk[sa[i]] == rk[sa[i-1]] && sa[i] + w <= n && sa[i-1] + w <= n && rk[sa[i] + w] == rk[sa[i-1] + w]);
        std::swap(tprk, rk);
        if (sigma == n) break;
    }
    if (rk != rk_) memcpy(rk_, rk, sizeof(int) * (n+1));
    for (int i = 1, k = 0; i <= n; i++) {
        if (k) --k;
        while (i + k <= n && sa[rk[i]-1] + k <= n && s[i + k] == s[sa[rk[i]-1] + k]) ++k;
        height[i] = k;
    }
}
int sa[N], rk[N], h[N], len[N];
char s[N], base[N], frontLeft[N], str[N]; // base is non decreasing
inline void solveSingleTestCase() {
    scanf("%s", s + 1);
    int n = strlen(s + 1);
    int lastMinPos = 0; char minChar = '\0';
    for (int i = n; i >= 1; i--)
        if (s[i] < minChar) minChar = s[i], lastMinPos = i;
/* [Reverse Ordered Increasing String "abcd"] <==Insert Into== [Borders and Min Suffixes "b", "bdb", "bdbccbdb"] + [Left String (front) + (back)]
    a  b  c     d  <== "b", "bcb", "bcbddbcb"
(0)                    abcdbcbddbcb *
(1)  b             ==> abbcdbcbddbc *
(2)  b  cb         ==> abbcbcdbcbdd *
(3)  b  cb ddbcb   ==> abbcbcddbcbd
*/
    int baseLen = 1, frontLeftLen = 0;
    base[1] = s[1];
    for (int i = 2; i <= n; i++) // 构造 base(操作一) 和 frontLeft(操作二)
        if (s[i] <= base[baseLen]) base[++baseLen] = s[i];
        else frontLeft[++frontLeftLen] = s[i];
    int frontLen = baseLen + frontLeftLen;
    getSA(n - frontLen, s+frontLen, sa, rk, h);
    int bdcnt = 0;
    for (int i = 1; i <= n; i++) {
        if (i > 1 && h[i] != n - frontLen - sa[i - 1] + 1) break; // 当 sa[i-1] 是 sa[i] 的 border 的时候才考虑将 sa[i] 设置为 insert string
        len[++bdcnt] = h[i];
    }
    memcpy(str+1, base+1, baseLen);
    memcpy(str+baseLen+1, frontLeft+1, frontLeftLen);
    memcpy(str+frontLen+1, s+frontLen+1, n - frontLen); // 构造 [base]+[other]
    getSA(n, s, sa, rk, h);
    int cmpStrPtr = 0;
    for (int i = 1; i <= bdcnt; i++) {
        
    }
}
int main() {
    int testCases;
    scanf("%d", &testCases);
    while (testCases--) solveSingleTestCase();
    return 0;
}