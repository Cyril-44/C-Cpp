#include <string.h>
#include <stdio.h>
#include <algorithm>
#ifdef DEBUG
# define DebugInfo(...) fprintf(stderr, __VA_ARGS__)
#else
# define DebugInfo(...) ((void)0)
#endif
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
    for (int i = 1; i <= n; i++) tpsa[i] = i, rk[i] = s[i];
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
        if (rk[i] == 1) {height[rk[i]] = 0; continue;}
        if (k) --k;
        while (i + k <= n && sa[rk[i]-1] + k <= n && s[i + k] == s[sa[rk[i]-1] + k]) ++k;
        height[rk[i]] = k;
    }
}
int sa[N], rk[N], h[N], len[N];
char s[N], baseStr[N], frontLeftStr[N], str[N], bestStr[N]; // base is non decreasing
inline void solveSingleTestCase() {
    scanf("%s", s + 1);
    int n = strlen(s + 1);
    int lastMinPos = 0; char minChar = 127;
    for (int i = n; i >= 1; i--)
        if (s[i] < minChar) minChar = s[i], lastMinPos = i;
    DebugInfo("[0] Detected last occured min character '%c' on pos %d\n", minChar, lastMinPos);
/* [Reverse Ordered Increasing String "abcd"] <==Insert Into== [Borders and Min Suffixes "b", "bdb", "bdbccbdb"] + [Left String (front) + (back)]
    a  b  c     d  <== "b", "bcb", "bcbddbcb"
(0)                    abcdbcbddbcb *
(1)  b             ==> abbcdbcbddbc *
(2)  b  cb         ==> abbcbcdbcbdd *
(3)  b  cb ddbcb   ==> abbcbcddbcbd
*/
    int baseLen = 1, frontLeftLen = 0;
    baseStr[1] = s[1];
    for (int i = 2; i <= lastMinPos; i++) // 构造 base(操作一) 和 frontLeft(操作二)
        if (s[i] <= baseStr[baseLen]) baseStr[++baseLen] = s[i];
        else frontLeftStr[++frontLeftLen] = s[i];
    std::reverse(baseStr+1, baseStr+1 + baseLen);
    int frontLen = baseLen + frontLeftLen;
    DebugInfo("[1] Calculated Base Str \"%s\" , Front Left Str \"%s\", Front Len %d\n", (baseStr[baseLen+1]=0, baseStr+1), (frontLeftStr[frontLeftLen+1]=0, frontLeftStr+1), frontLen);
    DebugInfo("[2] Sending \"%s\" to SA Sorting...\n", s + frontLen+1);
    getSA(n - frontLen, s+frontLen, sa, rk, h);
    DebugInfo("[2.1] SA("); for (int i = 1; i <= n - frontLen; i++) DebugInfo("%d%c", sa[i], i^n - frontLen?',':')'); DebugInfo(" RK("); for (int i = 1; i <= n - frontLen; i++) DebugInfo("%d%c", rk[i], i^n - frontLen?',':')');DebugInfo(" Height("); for (int i = 1; i <= n - frontLen; i++) DebugInfo("%d%c", h[i], i^n - frontLen?',':')'); DebugInfo("\n");
    int bdcnt = 1;
    len[1] = n - frontLen - sa[1] + 1;
    for (int i = 2; i <= n - frontLen; i++) {
        if (h[i] != n - frontLen - sa[i-1] + 1) break; // 当 sa[i-1] 是 sa[i] 的 border 的时候才考虑将 sa[i] 设置为 insert string
        len[++bdcnt] = n - frontLen - sa[i] + 1;
    }
    DebugInfo("[3] Borders (total: %d): ", bdcnt); for (int i = 1; i <= bdcnt; i++) DebugInfo("%d ", len[i]); DebugInfo("\n");
    memcpy(str+1, baseStr+1, baseLen);
    memcpy(str+baseLen+1, frontLeftStr+1, frontLeftLen);
    if (n == frontLen) {
        str[n+1] = '\0', puts(str+1);
        DebugInfo("[!] cuz frontLen = n, output directly...\n");
        return;
    }
    memcpy(str+frontLen+1, s+frontLen+1, n - frontLen); // 构造 [base]+[other]
    DebugInfo("[4] Constructed Str \"%s\"\n", (str[n+1]=0, str+1));
    getSA(n, str, sa, rk, h);
    int cmpBaseIdx = 1, bestBorderLen = 0;
    for (int i = 1, cmpIdx; i <= bdcnt; i++) { // 依次枚举 border，保证 border_{i-1} \in border_i
    /*    cmpBaseIdx （两边同时选，比较）
          |
          |       borderIdx
          ↓       ↓
        aaacddeeeedd
             ↑
             cmpIdx （原串）
    */
        int cmpBorderIdx = n - (len[i] - len[i-1]) + 1;
        bool isBetter = false, isWorse = false;
        for (cmpIdx = cmpBaseIdx; cmpBorderIdx <= n; ++cmpIdx) {
            DebugInfo("[4.1] {%d, %d} Base '%c'(#%d) and Border '%c'(#%d)", i, cmpIdx, str[cmpBaseIdx], cmpBaseIdx, str[cmpBorderIdx], cmpBorderIdx);
            int now = (cmpBaseIdx <= baseLen && str[cmpBaseIdx] < str[cmpBorderIdx] ? cmpBaseIdx : cmpBorderIdx)++;
            DebugInfo(", chosen pos %d\n", now);
            if (!isBetter && !isWorse) {
                if (str[now] < str[cmpIdx]) isBetter = true;
                else if (str[now] > str[cmpIdx]) isWorse = true;
            }
            DebugInfo("[4.2] {%d, %d} Comparing '%c' and '%c'(#%d)\n", i, cmpIdx, str[cmpIdx], str[now], now);
        }
        if (isWorse) break;
        else isBetter |= rk[cmpIdx] > rk[cmpBaseIdx] || cmpIdx == cmpBaseIdx;
        if (isBetter) bestBorderLen = len[i];
        DebugInfo("[4.3] {%d} Current Border Len %d, judged %s\n", i, len[i], isBetter ? "isBetter" : isWorse ? "worse&killed" : "tie");
    }
    DebugInfo("[5] Calculated best Border Len %d\n", bestBorderLen);
    cmpBaseIdx = 1; int cmpBorderIdx = n - bestBorderLen + 1;
    for (int i = 0; cmpBorderIdx <= n; i++) { // 根据 bestBorderLen 模拟建出最优字符串
        int now = (cmpBaseIdx <= baseLen && str[cmpBaseIdx] < str[cmpBorderIdx] ? cmpBaseIdx : cmpBorderIdx)++;
        bestStr[i] = str[now];
    }
    DebugInfo("[6] Simulating greedy algo, whose BaseIdx=%d, BorderIdx=%d\n", cmpBaseIdx, cmpBorderIdx);
    memcpy(bestStr + cmpBaseIdx + bestBorderLen - 1, str + cmpBaseIdx, n - (cmpBaseIdx - 1) - bestBorderLen);
    bestStr[n] = '\0';
    puts(bestStr);
}
int main() {
    int testCases;
    scanf("%d", &testCases);
    while (testCases--) solveSingleTestCase();
    return 0;
}
/* Hacks
dcddcccedd  =>>  ccccddddde
abbeaedabe  =>>  aaabbeedbe
cddadc      =>>  accddd
edeabcddb   =>>  abdeebcdd
*/