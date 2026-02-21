#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <set>
#include <vector>
#include <array>
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
std::pair<int,int> range[N];
std::vector<int> add1[N], del1[N], add2[N], del2[N];
inline void umin(int &x, int y) { x = std::min(x, y); }
inline void umax(int &x, int y) { x = std::max(x, y); }
int main() {
    int tid, n, q;
    scanf("%d%d%d %s", &tid, &n, &q, s + 1);
    for (int l, r; q--; ) {
        scanf("%d%d", &l, &r);
        int m = r - l + 1;
        for (int i = 1; i <= m; i++)
            add1[i].clear(), add2[i].clear(), del1[i].clear(), del2[i].clear();
        getsa(m, s + l - 1, sa, rk, height);
        for (int i = 1; i <= m; i++) range[i] = {m+1, 0};
        for (int i = 2; i <= m; i++) {
            umin(range[height[i]].first, std::min(sa[i-1], sa[i]));
            umax(range[height[i]].second, std::max(sa[i-1], sa[i]));
        }
        for (int i = m-1; i >= 1; i--) {
            umin(range[i].first, range[i+1].first);
            umax(range[i].second, range[i+1].second);
        }
        std::multiset<int> val1, val2; // 1维护一次函数，2维护常函数
        for (int h = 1; h <= m; h++) {
            int pl = range[h].first, pr = range[h].second;
            if (pl > pr) break;
            int incR = std::min(pr + 1, pl + h);
            add1[pl].push_back(1 - pl);
            del1[incR].push_back(1 - pl);
            if (pl + h <= pr) {
                add2[pl + h].push_back(h);
                del2[pr + 1].push_back(h);
            }
            // fprintf(stderr, "p=[%d,%d] h=%d\n", pl, pr, h);
        }
        int ans = 0;
        val1.insert(-m), val2.insert(0);
        for (int i = 1; i <= m; i++) {
            for (int x : add1[i]) val1.insert(x);
            for (int x : del1[i]) val1.erase(val1.find(x));
            for (int x : add2[i]) val2.insert(x);
            for (int x : del2[i]) val2.erase(val2.find(x));
            ans ^= i + std::max({1, *val1.rbegin() + i, *val2.rbegin()});
            // fprintf(stderr, "(%d,%d) ", *val1.rbegin() + i, *val2.rbegin());
        }
        printf("%d\n", ans);
    }
    return 0;
}