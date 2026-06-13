#include <cstdio>
#include <string>
#include <iostream>
#include <cstdint>
#include <vector>
#include <cstring>
#include <numeric>
#include <algorithm>
constexpr int N = 100005;
constexpr unsigned MOD = ~0u;
std::string s[N];
int n;
inline int64_t calcbf() {
    int64_t ans = 0;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j < i; j++) {
            int f = 0, g = 0, mx = std::min(s[i].length(), s[j].length());
            while (f < mx && s[i][f] == s[j][f]) ++f;
            while (g < mx && s[i][s[i].length()-g-1] == s[j][s[j].length()-g-1]) ++g;
            fprintf(stderr, "%d*%d\n", f, g);
            ans += f * g;
        }
    return ans;
}
namespace {
constexpr int N = 200005, LG = 17;
char str[N], rstr[N];
using STArr = int[N][LG+1];
int sa[N], rk[N], h[N]; STArr lcpst;
int rsa[N], rrk[N], rh[N]; STArr rlcpst;
inline void getSA(int n, char *s, int *sa, int *rk_, int *height, STArr st, int sigma = 127) {
    static int tsa[N], h[N], cnt[N];
    int *rk = rk_, *trk = h;
    auto radixSort = [&]() { 
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
            trk[sa[i]] = sigma += !(rk[sa[i]] == rk[sa[i-1]] && (sa[i]+w <= n && sa[i-1]+w <= n && rk[sa[i] + w] == rk[sa[i-1] + w]));
        std::swap(rk, trk);
    }
    if (rk != rk_) memcpy(rk_+1, rk+1, sizeof(int) * n);
    for (int i = 1, k = 0; i <= n; i++) {
        if (rk[i] == 1) continue;
        if (k) --k;
        while (i+k<=n && sa[rk[i]-1]+k<=n && s[i + k] == s[sa[rk[i] - 1] + k]) ++k;
        height[rk[i]] = k;
    }
    for (int i = 1; i <= n; i++) st[i][0] = height[i];
    for (int k = 1; k <= LG; k++)
        for (int i = 1, rg = n - (1 << k) + 1; i <= rg; i++)
            st[i][k] = std::min(st[i][k-1], st[i+(1<<k-1)][k-1]);
}
inline int lcp(int l, int r) {
    l = rk[l], r = rk[r];
    if (l > r) std::swap(l, r);
    int k = 31 - __builtin_clz(r - l++);
    return std::min(lcpst[l][k], lcpst[r-(1<<k)+1][k]);
}
inline int rlcp(int l, int r) {
    l = rrk[l], r = rrk[r];
    if (l > r) std::swap(l, r);
    int k = 31 - __builtin_clz(r - l++);
    return std::min(rlcpst[l][k], rlcpst[r-(1<<k)+1][k]);
}
int pos[N];
namespace BF {
inline int64_t solve() {
    int cur = 1;
    for (int i = 1; i <= n; i++) {
        pos[i] = cur;
        memcpy(str+cur, s[i].data(), s[i].length());
        std::reverse(s[i].begin(), s[i].end());
        memcpy(rstr+cur, s[i].data(), s[i].length());
        cur += s[i].length();
        str[cur] = rstr[cur] = '$';
        ++cur;
    }
    str[cur] = rstr[cur] = '\0';
    // puts(str+1);
    // puts(rstr+1);
    --cur; getSA(cur, str, sa, rk, h, lcpst); getSA(cur, rstr, rsa, rrk, rh, rlcpst);
    // for (int i = 1; i <= cur; i++) fprintf(stderr, "%d %d %d %d %d %d\n", sa[i], rk[i], h[i], rsa[i], rrk[i], rh[i]);
    int64_t ans = 0;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j < i; j++) {
            int mx = std::min(s[i].length(), s[j].length());
            int f = std::min(mx, lcp(pos[i], pos[j])),
                g = std::min(mx, rlcp(pos[i], pos[j]));
            // fprintf(stderr, "pos{%d,%d} %d*%d\n", pos[i], pos[j], f, g);
            ans += 1ll*f*g;
        }
    return ans;
}
}}
namespace { namespace Sub1 {
int64_t ans;
struct Node {
    int ch[26], cnt;
} tr[N];
int tot;
inline int alloc() {
    tr[++tot] = {};
    return tot;
}
inline void check(const std::string &s) {
    int u = 0;
    std::vector<int> cnt(s.size());
    for (int i = 0; i < (int)s.length(); i++) {
        if (!tr[u].ch[s[i]-'a']) tr[u].ch[s[i]-'a'] = alloc();
        u = tr[u].ch[s[i]-'a'];
        cnt[i] = tr[u].cnt;
        ++tr[u].cnt;
    }
    for (int i = 1; i < (int)s.length(); i++) cnt[i-1] -= cnt[i];
    for (int i = 0; i < (int)s.length(); i++) ans += cnt[i] * (i+1ll)*(i+1ll);
}
inline int64_t solve() {
    ans = 0; tot = 0; tr[0] = {};
    for (int i = 1; i <= n; i++) {
        check(s[i]);
    }
    return ans;
}
}}
int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    int T;
    std::cin >> T;
    while (T--) {
        std::cin >> n;
        for (int i = 1; i <= n; i++) std::cin >> s[i];
        bool flg = 1;
        for (int i = 1; i <= n; i++) {
            std::string tmp = s[i];
            std::reverse(tmp.begin(), tmp.end());
            if (tmp != s[i]) {flg = 0; break;}
        }
        if (flg)printf("%u\n", unsigned(Sub1::solve()));
        else printf("%u\n", unsigned(BF::solve() % MOD));
         
    }
    return 0;
}
