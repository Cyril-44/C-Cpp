#include <cstdio>
#include <cstring>
#include <algorithm>
#include <numeric>
#include <set>
#include <unordered_map>
#include <cstdint>
#ifdef DEBUG
#  define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#  define debug(...) ((void)0)
#endif
constexpr int N = 200005, LGN = 18;
constexpr uint64_t B = 31;
char s[N], revs[N];
int n;
uint64_t pwB[N], hash[N];
struct SuffixArray {
    inline void init(int n, char *s, int sigma = 127) {
        this->n = n;
        int *rk_ = rk, *oldrk = _tp;
        auto radixSort = [&]() {
            memset(cnt, 0, sizeof(int) * (sigma + 1));
            for (int i = 1; i <= n; i++) ++cnt[rk_[i]];
            for (int i = 1; i <= sigma; i++) cnt[i] += cnt[i-1];
            for (int i = n; i >= 1; i--) sa[cnt[rk_[ssa[i]]]--] = ssa[i];
        };
        rk[n+1] = _tp[n+1] = 0; // 最多多访问一个
        for (int i = 1; i <= n; i++) rk_[i] = s[i], ssa[i] = i;
        radixSort();
        for (int w = 1; w < n; w <<= 1) {
            int ord = 0;
            for (int i = n - w + 1; i <= n; i++) ssa[++ord] = i;
            for (int i = 1; i <= n; i++) if (sa[i] > w) ssa[++ord] = sa[i] - w;
            radixSort();
            std::swap(rk_, oldrk);
            sigma = 0;
            for (int i = 1; i <= n; i++)
                rk_[sa[i]] = sigma += !(oldrk[sa[i]] == oldrk[sa[i-1]] && oldrk[sa[i]+w] == oldrk[sa[i-1]+w]);
            if (sigma == n) break;
        }
        for (int i = 1, w = 0; i <= n; i++) {
            if (rk_[i] == 1) continue;
            if (w) --w;
            while (i+w <= n && sa[rk_[i]-1]+w <= n && s[i+w] == s[sa[rk_[i]-1]+w]) ++w;
            h[rk_[i]] = w;
        }
        if (rk_ != rk) memcpy(rk +1, rk_+1, sizeof(int) * n);
        for (int i = 1; i <= n; i++) st[0][i] = h[i];
        for (int k = 1; k <= LGN; k++)
            for (int i = 1, j = 1 + (1<<k-1), l = (1<<k); l <= n; i++, j++, l++)
                st[k][i] = std::min(st[k-1][i], st[k-1][j]);
    }
    int _tp[N], ssa[N], cnt[N];
    int sa[N], rk[N], h[N];
    int st[LGN+1][N];
    int n;
    int operator()(int l, int r) const {
        // if (l == r) return n - l + 1;
        l = rk[l], r = rk[r];
        if (l > r) std::swap(l, r);
        int k = 31 - __builtin_clz(r - l++);
        return std::min(st[k][l], st[k][r-(1<<k)+1]);
    }
};
SuffixArray saP, saS;
/* namespace BruteForce {
    static std::unordered_set<uint64_t> all;
    static inline void kmp(int n, char *s) {
        static int f[N];
        for (int i = 1; i <= n; i++) {
            int j = f[i-1];
            while (j && s[j+1] != s[i]) j = f[j];
            if (j+1 < i && s[j+1] == s[i]) ++j;
            f[i] = j;
        }
        uint64_t now = 0;
        for (int i = 1; i <= n; i++) {
            now = now * B + (s[i] - 'a') + 1;
            if (f[i] && i % (i - f[i]) == 0) all.insert(now);
        }
    }
    inline void work(const int max = 0x7fffffff) {
        long long ans = n*(n+1ll)/2 - std::accumulate(h+1, h+1+n, 0ll);
        std::unordered_set<uint64_t>().swap(all);
        for (int i = 1; i <= n; i++)
            kmp(std::min(n - i + 1, max), s + i - 1);
        ans -= all.size();
        printf("%lld\n", ans);
    }
} */
inline void initHash() {
    for (int i = 1; i <= n; i++)
        hash[i] = hash[i-1] * B + (s[i] - 'a' + 1);
}
inline uint64_t getHash(int l, int r) {
    --l;
    return hash[r] - hash[l] * pwB[r - l];
}
inline void umax(int &x, int y) { (x < y) && (x = y); }
inline int lcp(int l, int r) { return saP(l, r); }
inline int lcs(int l, int r) { return saS(n-l+1, n-r+1); }
template<>struct std::hash<std::pair<int,int>> { inline uint64_t operator()(std::pair<int,int> p) const { return (uint64_t)p.first << 32 | p.second; } };
inline void solveSingle() {
    scanf("%d %s", &n, s+1);
    initHash();
    std::reverse_copy(s+1, s+1+n, revs+1);
    saP.init(n, s);
    saS.init(n, revs);
    std::set<std::pair<int,int>> occurs;
    std::unordered_map<uint64_t, int> mp;
    // size_t loop_count = 0;
    for (int len = 1; len*2 <= n; len++) {
        for (int i = len, j = len+len; j <= n; i += len, j += len) {
            // ++loop_count;
            int rs = lcp(i, j), ls = lcs(i-1, j-1);
            int l = i - ls, r = j-1 + rs;
            if (ls + rs >= len) {
                if (r >= j+len) { // Goal: let j > r
                    int add = (r - j - len) / len * len;
                    i += add, j += add;
                }
                if (occurs.emplace(l, r).second)
                    for (int w = ls + rs, cnt = len; cnt-- && w >= len; w--, l++) {
                        // ++loop_count;
                        umax(mp[getHash(l, l+len-1)], w / len);
                        // debug("len=%d, i=%d, l=%d, wish to upd 0x%llX to+ %d (w=%d)\n", len, i, l, getHash(l, l+len-1), (ls+rs)/len, w);
                    }
            }
        }
    }
    long long ans = n*(n+1ll)/2 - std::accumulate(saP.h+1, saP.h+1+n, 0ll);
    // debug("SubString Count=%lld\n", ans);
    // fprintf(stderr, "Loops: %zu\n", loop_count);
    for (const auto &[x, y] : mp) {
        ans -= y;
        debug("Structure like 0x%llX gives %d\n", x, y);
    }
    printf("%lld\n", ans);
}
int main() {
    pwB[0] = 1;
    for (int i = 1; i <= 200000; i++) pwB[i] = pwB[i-1] * B;
    int T;
    scanf("%d", &T);
    while (T--) solveSingle();
    return 0;
}