#include <cstdio>
#include <cstring>
#include <algorithm>
#include <unordered_set>
#include <vector>
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
} saP, saS;
inline int lcp(int l, int r) { return saP(l, r); }
inline int lcs(int l, int r) { return saS(n-l+1, n-r+1); }
template<> struct std::hash<std::pair<int,int>> { size_t operator()(std::pair<int,int> p) const { return 1ull*p.first << 32 | 1u*p.second; } };
struct SegTr {
    struct Node {
        long long sum, add;
        void pull(long long v, int len) { sum += v * len, add += v; }
    } tr[N << 2];
    int L, R, X;
    void pushdown(int u, int ls, int rs) {
        if (tr[u].add) {
            tr[u<<1].pull(tr[u].add, ls);
            tr[u<<1|1].pull(tr[u].add, rs);
            tr[u].add = 0;
        }
    }
    void pushup(int u) {
        tr[u].sum = tr[u<<1].sum + tr[u<<1|1].sum;
    }
    void upd(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].pull(X, r-l+1);
        int mid = l + r >> 1;
        pushdown(u, mid-l+1, r-mid);
        if (L <= mid) upd(u<<1, l, mid);
        if (mid < R) upd(u<<1|1, mid+1, r);
        pushup(u);
    }
    long long inq(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].sum;
        long long res = 0; int mid = l + r >> 1;
        pushdown(u, mid-l+1, r-mid);
        if (L <= mid) res += inq(u<<1, l, mid);
        if (mid < R) res += inq(u<<1|1, mid+1, r);
        return res;
    }
    void update(int l, int r, int x) { L=l, R=r, X=x; upd(1,1,n); }
    long long inquire(int l, int r) { L=l, R=r; return inq(1,1,n); }
} fl, fr;
std::vector<std::pair<int,int>> modl[N], modr[N], quel[N], quer[N];
long long ans[N];
/*
好唐的题。用《优秀的拆分》思路，找到所有的2重串，然后发现是一个平面直角坐标系上的查询问题。
具体来说，我们的2重串分布在 nlogn 条，在原串区间形如 [l,r], [l+1,r+1], ... [l+w,r+2]。
我们以x轴为 "l"，y轴为 "len"。然后发现，贡献就是水平线段。查询的东西就是一个三角，形如：
^ (len)
|                这样的查询，我们可以拆成一个矩形减去一个平行四边形，也就是，
|  |\ ----       左边的竖线一直向右延伸的贡献 - 右边的斜线一直向右延伸的贡献。
| -|-\---        后面那个要把坐标系扭一下，然后发现也可以转换为 x轴表示 "r"，两个扫描线，Seg 维护历史和，答案差分计算即可。
| -|--\---     
|__|___\__________> (l)
*/
int main() {
    int q;
    scanf("%d%d %s", &n, &q, s+1);
    std::reverse_copy(s+1, s+1+n, revs+1);
    saP.init(n, s);
    saS.init(n, revs);
    for (int len = 1; len*2 <= n; len++) {
        std::unordered_set<std::pair<int,int>> occurs;
        for (int i = len, j = len+len; j <= n; i += len, j += len) {
            int rs = lcp(i, j), ls = lcs(i-1, j-1);
            int l = i - ls, r = j-1 + rs;
            if (ls + rs >= len) {
                if (r >= j+len) { // Goal: let j > r
                    int add = (r - j - len) / len * len;
                    i += add, j += add;
                }
                if (occurs.emplace(l, r).second) {
                    modl[len*2].emplace_back(l, r-2*len+1);
                    modr[len*2].emplace_back(l+2*len-1, r);
                }
            }
        }
    }
    for (int i = 1, l, r; i <= q; i++) {
        scanf("%d%d", &l, &r);
        quel[r-l+1].emplace_back(l, i);
        if (r < n) quer[r-l+1].emplace_back(r+1, i);
    }
    for (int len = 1; len <= n; len++) {
        // fprintf(stderr, "At len=%d\n", len);
        for (auto [l, r] : modl[len]) fl.update(l, r, 1);
        for (auto [l, r] : modr[len]) fr.update(l, r, 1);
        for (auto [l, idx] : quel[len]) ans[idx] += fl.inquire(l, n);
        for (auto [l, idx] : quer[len]) ans[idx] -= fr.inquire(l, n);
    }
    for (int i = 1; i <= q; i++)
        printf("%lld\n", ans[i]);
    return 0;
}