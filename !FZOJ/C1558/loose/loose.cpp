#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <set>
#include <vector>
#pragma GCC optimize(2)
#pragma GCC optimize("unroll-loops", "inline", "no-stack-protector")
constexpr int N = 100005;
char s[N];
int sa[N], rk[N], height[N];
inline void getsa(int n, char *s, int *sa, int *rk_, int *height, int sigma = 127) {
    static int h[N], tsa[N], cnt[N];
    int *rk = rk_, *trk = h;
    auto radixSort = [&]() { // 按照 tsa[] 给定的相等比较顺序，根据 rk 进行排名
        memset(cnt, 0, sizeof(int) * (sigma + 1));
        for (int i = 1; i <= n; i++) ++cnt[rk[i]];
        for (int i = 1; i <= sigma; i++) cnt[i] += cnt[i-1];
        for (int i = n; i >= 1; i--) sa[cnt[rk[tsa[i]]]--] = tsa[i];
    };
    for (int i = 1; i <= n; i++) tsa[i] = i, rk[i] = s[i];
    radixSort();
    for (int w = 1; w < n; w <<= 1) {
        int top = 0;
        for (int i = n - w + 1; i <= n; i++) tsa[++top] = i;
        for (int i = 1; i <= n; i++) if (sa[i] > w) tsa[++top] = sa[i] - w;
        radixSort();
        sigma = 0;
        for (int i = 1; i <= n; i++)
            trk[sa[i]] = sigma += !(rk[sa[i]] == rk[sa[i-1]] && sa[i]+w <= n && sa[i-1]+w <= n && rk[sa[i] + w] == rk[sa[i-1] + w]);
        std::swap(rk, trk);
        if (sigma == n) break;
    }
    if (rk != rk_) memcpy(rk_+1, rk+1, sizeof(int) * n);
    for (int i = 1, k = 0; i <= n; i++) {
        if (rk[i] == 1) continue;
        if (k) --k;
        while (i+k <= n && sa[rk[i]-1]+k <= n && s[i+k] == s[sa[rk[i]-1]+k]) ++k;
        height[rk[i]] = k;
    }
}
namespace PSGT {
int mx[N * 50];
int tot;
void upd(){
    
}
}
std::vector<int> add1[N], del1[N], add2[N], del2[N];
int main() {
    int tid, n, q;
    scanf("%d%d%d %s", &tid, &n, &q, s + 1);
    for (int l, r; q--; ) {
        scanf("%d%d", &l, &r);
        int m = r - l + 1;
        for (int i = 1; i <= m; i++)
            add1[i].clear(), add2[i].clear(), del1[i].clear(), del2[i].clear();
        getsa(m, s + l - 1, sa, rk, height);
        std::multiset<int> val1, val2; // 1维护一次函数，2维护常函数
        for (int i = 2; i <= m; i++) {
            int p1 = sa[i-1], p2 = sa[i];
            if (p1 > p2) std::swap(p1, p2);
            int incR = std::min(p2 + 1, p1 + height[i]);
            add1[p1].push_back(1 - p1);
            del1[incR].push_back(1 - p1);
            if (p1 + height[i] <= p2) {
                add2[p1 + height[i]].push_back(height[i]);
                del2[p2 + 1].push_back(height[i]);
            }
            fprintf(stderr, "p1=%d, p2=%d, h=%d\n", p1, p2, height[i]);
        }
        int ans = 0;
        val1.clear(), val2.clear();
        val1.insert(-m), val2.insert(0);
        for (int i = 1; i <= m; i++) {
            for (int x : add1[i]) val1.insert(x);
            for (int x : del1[i]) val1.erase(val1.find(x));
            for (int x : add2[i]) val2.insert(x);
            for (int x : del2[i]) val2.erase(val2.find(x));
            ans ^= i + std::max({1, *val1.rbegin() + i, *val2.rbegin()});
            fprintf(stderr, "(%d,%d) ", *val1.rbegin() + i, *val2.rbegin());
        }
        fprintf(stderr, "\n");
        val1.clear(), val2.clear();
        val1.insert(-m), val2.insert(0);
        for (int i = 1; i <= m; i++) {
            for (int x : add1[i]) val1.insert(x);
            for (int x : del1[i]) val1.erase(val1.find(x));
            for (int x : add2[i]) val2.insert(x);
            for (int x : del2[i]) val2.erase(val2.find(x));
            fprintf(stderr, "%d ", std::max({1, *val1.rbegin() + i, *val2.rbegin()}));
        }
        fprintf(stderr, "\n");
        printf("%d\n", ans);
    }
    return 0;
}