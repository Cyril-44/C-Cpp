#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <array>
constexpr int N = 200005, MXK = 20;
char s[N];
int sa[N], rk[N << 1], height[N];
inline void initSA(int n, char *s, int *sa, int *rk_, int sigma = 127) {
    static int a[N], b[N << 1], cnt[N];
    int *tsa = a, *trk = b, *rk = rk_;
    for (int i = 1; i <= n; i++) tsa[i] = i, rk[i] = s[i];
    auto radixSort = [&]() { // Using ptr "tsa" and key "rk" to gen "sa"
        memset(cnt, 0, sizeof(int) * (sigma + 1));
        for (int i = 1; i <= n; i++) ++cnt[rk[i]];
        for (int i = 1; i <= sigma; i++) cnt[i] += cnt[i-1];
        for (int i = n; i >= 1; i--) sa[cnt[rk[tsa[i]]]--] = tsa[i];
    };
    radixSort();
    for (int w = 1; w < n; w <<= 1) {
        int top = 0;
        for (int i = n - w + 1; i <= n; i++) tsa[++top] = i;
        for (int i = 1; i <= n; i++) if (sa[i] > w) tsa[++top] = sa[i] - w;
        radixSort();
        sigma = 0;
        for (int i = 1; i <= n; i++)
            trk[sa[i]] = sigma += !(rk[sa[i]] == rk[sa[i-1]] && rk[sa[i] + w] == rk[sa[i-1] + w]);
        std::swap(rk, trk);
        if (sigma == n) break;
    }
    if (rk != rk_) memcpy(rk_, rk, sizeof(int) * (n+1));
}
namespace LCP{
int n;
std::vector<std::array<int, MXK>> st;
inline void initLCP(int n, char *s) { // height_i = lcp(sa_i, sa_{i-1})
    LCP::n = n;
    for (int i = 1, curr = 0; i <= n; i++) {
        if (curr) --curr;
        while (s[i + curr] == s[sa[rk[i] - 1] + curr]) ++curr;
        height[rk[i]] = curr;
    }
    st.assign(n + 2, {});
    for (int i = 1; i <= n; i++) st[i][0] = height[i];
    for (int k = 1; k < MXK; ++k)
        for (int i = 0; i + (1 << k) - 2 <= n; ++i)
            st[i][k] = std::min(st[i][k-1], st[i + (1 << k-1)][k-1]);
}
inline int lcp(int i, int j) { // lcp(sa_i, sa_j) = min(height_{i+1}, j)
    if (i == j) return n - sa[i] + 1;
    ++i;
    int k = 31 - __builtin_clz(j - i + 1);
    return std::min(st[i][k], st[j - (1 << k) + 1][k]);
}
} using LCP::lcp; using LCP::initLCP;

struct PSGT {
    struct Node { int ls, rs, mx; Node() : ls(), rs(), mx() {} };
    inline PSGT(int n) : tr(1), n(n), root(n+1) { tr.reserve((n+1) * 25); }
    int& operator[](int x) { return root[x]; }
    inline void update(int pos, int val) {
        root[pos] = root[pos - 1];
        P = val, X = pos;
        upd(root[pos], 1, n);
    }
    inline int lower(int posl, int posr, int val) {
        X = posl, P = val;
        return quepre(root[posr], 1, n);
    }
    inline int upper(int posl, int posr, int val) {
        X = posl, P = val;
        return quesuf(root[posr], 1, n);
    }
private:
    std::vector<Node> tr;
    std::vector<int> root;
    int P, X, n;
    inline void pushup(int u) { tr[u].mx = std::max(tr[tr[u].ls].mx, tr[tr[u].rs].mx); }
    void upd(int &u, int l, int r) {
        tr.emplace_back(tr[u]);
        u = tr.size() - 1;
        if (l == r) { tr[u].mx = X; return; }
        int mid = l + r >> 1;
        if (P <= mid) upd(tr[u].ls, l, mid);
        else upd(tr[u].rs, mid+1, r);
        pushup(u);
    }
    int quepre(int u, int l, int r) {
        if (!u || tr[u].mx < X) return 0;
        if (l == r) return l;
        int res = 0, mid = l + r >> 1;
        if (mid < P) res = quepre(tr[u].rs, mid + 1, r);
        if (res) return res; // 优先更大的 右子树
        return quepre(tr[u].ls, l, mid);
    }
    int quesuf(int u, int l, int r) {
        if (!u || tr[u].mx < X) return n+1;
        if (l == r) return l;
        int res = n+1, mid = l + r >> 1;
        if (mid >= P) res = quesuf(tr[u].ls, l, mid);
        if (res ^ n+1) return res; // 优先左子树
        return quesuf(tr[u].rs, mid + 1, r);
    }
};
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    scanf("%s", s + 1);
    PSGT f(n);
    initSA(n, s, sa, rk);
    initLCP(n, s);
    // for (int i = 1; i <= n; i++) printf("%d: sa=%d, rk=%d, height=%d\n", i, sa[i], rk[i], height[i]);
    for (int i = 1; i <= n; i++)
        f.update(i, rk[i]);
    auto calc = [&] (int l, int r, int x) -> int { // [l,r] 找 rk 离 x 最近的两个数，求max(lcp(a_i, x))
        int mn = f.lower(l, r, x), mx = f.upper(l, r, x);
        // printf("[%d, [%d], %d] %d, %d\n", mn, x, mx, lcp(mn, x), lcp(x, mx));
        return std::max(lcp(mn, x), lcp(x, mx));
    };
    for (int opt = 1, a, b, c, d; opt <= m; opt++) {
        scanf("%d%d%d%d", &a, &b, &c, &d);
        int l = 1, r = std::min(b - a, d - c) + 1, m;
        while (l <= r) {
            m = l + r >> 1;
            if (calc(a, b - m + 1, rk[c]) >= m) l = m + 1;
            else r = m - 1;
        }
        printf("%d\n", r);
    }
    return 0;
}
