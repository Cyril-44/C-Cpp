#include <bits/stdc++.h>
namespace FastI {
    char buf[1 << 20], *p1=buf, *p2=buf;
    inline char gc() {
        if (p1 == p2) {
            p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
            if (p1 == p2) return EOF;
        }
        return *p1++;
    }
    template<class T> inline void in(T &x) {
        char ch = gc();
        while (ch < '0' || ch > '9') ch = gc();
        for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
} using FastI::in;
#ifdef CLANGD
constexpr int N = 25;
#else
constexpr int N = 200005;
#endif
std::vector<int> g[N];
struct Mint {
    constexpr static int MOD = 998244353;
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    Mint (int v=0) : val(v) {}
    template<typename T> explicit operator T() const { return static_cast<T>(val); }
private: int val;
};
Mint qpow(Mint b, uint64_t n) {
    n %= Mint::MOD - 1;
    Mint res = 1;
    while (n) {
        if (n & 1) res *= b;
        b *= b, n >>= 1;
    }
    return res;
}
int n;
class SegTr {
    struct Node {
        int sum, ls, rs;
    } tr[N * 20];
    int rt[N], top, P, X, L, R;
    void upd(int &u, int l, int r) {
        if (!u) tr[u = ++top] = {};
        if (l == r) { tr[u].sum += X; return; }
        int mid = l + r >> 1;
        if (P <= mid) upd(tr[u].ls, l, mid);
        else upd(tr[u].rs, mid+1, r);
        tr[u].sum = tr[tr[u].ls].sum + tr[tr[u].rs].sum;
    }
    int inq(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].sum;
        int mid = l + r >> 1, res = 0;
        if (L <= mid) res += inq(tr[u].ls, l, mid);
        if (mid < R) res += inq(tr[u].rs, mid+1, r);
        return res;
    }
    int mer(int u, int v, int l, int r) {
        if (!u || !v) return u | v;
        tr[u].sum += tr[v].sum;
        if (l != r) {
            int mid = l + r >> 1;
            tr[u].ls = mer(tr[u].ls, tr[v].ls, l, mid);
            tr[u].rs = mer(tr[u].rs, tr[v].rs, mid+1, r);
        }
        return u;
    }
public:
    void clear() { memset(rt, 0, sizeof rt); top = 0; }
    void update(int &u, int p, int x) { P=p, X=x; upd(rt[u], 1, n); }
    int inquire(int u, int l, int r) { if (l > r) return 0; L=l, R=r; return inq(rt[u], 1, n); }
    void merge(int u, int v) { rt[u] = mer(rt[u], rt[v], 1, n); }
} fsum;
bool vis[N];
int ord[N], top; // 在下面的树中，u 编号排第几位
int gtCnt[N], gtCntFa[N]; // 子树中比当前点大的点，子树中比父亲大的点
void dfs1(int u) {
    vis[u] = true;
    fsum.update(u, u, 1);
    for (int v : g[u]) if (!vis[v]) {
        dfs1(v);
        gtCntFa[v] = fsum.inquire(v, u+1, n);
        fsum.merge(u, v);
    }
    gtCnt[u] = fsum.inquire(u, u+1, n);
}
int64_t f[N];
void dfs2(int u, int fa, int64_t cur) {
    cur -= gtCnt[u];
    f[u] = cur;
    cur += top - ord[u] - gtCnt[u];
    for (int v : g[u]) if (v != fa)
        dfs2(v, u, cur + gtCnt[u] - gtCntFa[v]);
}
int sum[N];
Mint ans3;
void dfs3(int u, int fa, int64_t cur) {
    // fprintf(stderr, "dfs3 %d: cur=%d\n", u, cur);
    ans3 += qpow(2, cur);
    for (int v : g[u]) if (v != fa) dfs3(v, u, cur + sum[v]);
}
int main() {
    int T;
    in(T);
    while (T--) {
        in(n);
        for (int i = 1; i <= n; i++) g[i].clear();
        for (int u, v, i = 2; i < n; i++) {
            in(u), in(v);
            g[u].push_back(v);
            g[v].push_back(u);
        }
        memset(vis, 0, sizeof vis);
        memset(ord, 0, sizeof ord);
        memset(f, 0, sizeof f);
        memset(sum, 0, sizeof sum);
        fsum.clear();
        dfs1(1);
        int64_t cnt1 = 0; // 上面树任意连树边的可连边数
        for (int i = 2; i <= n; i++)
            if (vis[i]) cnt1 += gtCnt[i];
        int sert = 0; top = 0;
        for (int i = 1; i <= n; i++)
            if (!vis[i]) sert = i, sum[i] = 1, ord[i] = ++top;
        int64_t cnt2 = 0; // 下面树任意连树边的可连边数
        dfs1(sert);
        for (int i = 1; i <= n; i++)
            if (ord[i]) cnt2 += gtCnt[i];
        dfs2(sert, 0, cnt2);
        Mint ans2 = 0; // 下面树任意连树边，以及任意往外一个点连边的方案数
        for (int i = 1; i <= n; i++) if (ord[i])
            ans2 += qpow(2, f[i] + top - ord[i]);
        sum[n+1] = 0;
        for (int i = n; i >= 1; i--) sum[i] += sum[i+1];
        ans3 = 0; // 下面连接到到根链的方案数
        dfs3(1, 0, 0);
        if (top == 1) ans3 += 1; // 下面可以不往上连边，因为是孤立的
        // fprintf(stderr, "2^%lld * %d * %d\n", cnt1, ans2, ans3);
        // for (int i = 1; i <= n; i++) if (ord[i])
        //     fprintf(stderr, "Spec Node %d (ord=%d): f=%lld\n", i, ord[i], f[i]);
        printf("%d\n", qpow(2, cnt1) * ans2 * ans3);
    }
    return 0;
}