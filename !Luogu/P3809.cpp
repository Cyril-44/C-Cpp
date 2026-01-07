#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>
constexpr int N = 1000005;
char s[N];
int sa[N]; // Suffix strings sorted Array (where x refers to substring x~n)
int rk[N << 2]; // suffix string i's RanK (i refers to substring i~n)
inline void getSA(int n, char *s, int *sa, int *rk_, int sigma = 127) {
    static int h[N], hh[N << 1], cnt[N];
    int *presa = h, *tprk = rk, *rk = hh;
    auto radixSort = [&] () { // 根据 presa(按照第二关键字排好序的下标) 和 rk(第一关键字) 将结果提取到 sa
        memset(cnt, 0, sizeof(int) * (sigma + 1));
        for (int i = 1; i <= n; i++) ++cnt[rk[i]];
        for (int i = 1; i <= sigma; i++) cnt[i] += cnt[i-1];
        for (int i = n; i >= 1; i--) sa[cnt[rk[presa[i]]]--] = presa[i]; // 这里不能直接计算 rk 因为是双关键字
    };

    for (int i = 1; i <= n; i++) rk[i] = s[i], presa[i] = i;
    radixSort();
    for (int w = 1; w < n; w <<= 1) { // 一次就按照 (rk[x], rk[x + w]) 排序
        // 按照第二关键字 rk[x + w] 排序的结果就是后面 w 个越界的放在 sa 最前面，其余的根据 sa[i] 一个一个放进来变成 sa[i]-w
        int top = 0;
        for (int i = n - w + 1; i <= n; i++) presa[++top] = i;
        for (int i = 1; i <= n; i++) if (sa[i] > w) presa[++top] = sa[i] - w;
        radixSort();
        // 根据得到的 sa(第一关键字排序结果) 和 rk 得到新的 rank 数组和 sigma
        sigma = 0;
        for (int i = 1; i <= n; i++) tprk[sa[i]] = sigma += !(rk[sa[i]] == rk[sa[i-1]] && (rk[sa[i] + w] == rk[sa[i-1] + w]));
        std::swap(tprk, rk);
        if (sigma == n) break;
    }
    if (rk != rk_) memcpy(rk_, rk, sizeof(int) * (n+1));
}
inline void getSA(int n) { getSA(n, s, sa, rk); }
int main() {
    scanf("%s", s + 1);
    int n = strlen(s+1);
    getSA(n);
    for (int i = 1; i <= n; i++)
        printf("%d ", sa[i]);
    putchar ('\n');
    return 0;
}