#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <stack>
#include <list>
constexpr int N = 100005, LGN = 25, MOD = (int)1e9 + 9;
std::vector<std::pair<int,int>> g[N];
struct ModInt {
    ModInt(int v=0) : val(v) {}
    ModInt operator-() const { return ModInt(!val ? 0 : MOD-val); }
    ModInt& operator+=(ModInt x) { if ((val += x.val) >= MOD) val -= MOD; return *this; }
    ModInt& operator-=(ModInt x) { if ((val -= x.val) < 0) val += MOD; return *this; }
    ModInt& operator*=(ModInt x) { val = 1ull*val*x.val % MOD; return *this; }
    ModInt operator+(ModInt x) const { return ModInt(*this) += x; }
    ModInt operator-(ModInt x) const { return ModInt(*this) -= x; }
    ModInt operator*(ModInt x) const { return ModInt(*this) *= x; }
    template<class T> constexpr explicit operator T() const { return static_cast<T>(val); }
    private: int val;
};
ModInt qpow(ModInt b, int n) {
    ModInt res = 1;
    while (n) {
        if (n & 1) res *= b;
        b *= b, n >>= 1;
    }
    return res;
}
static ModInt fact[N], ifact[N];
static struct _OnInit_Fact { _OnInit_Fact() {
    fact[0] = 1;
    for (int i = 1; i < N; i++) fact[i] = fact[i-1] * i;
    ifact[N-1] = qpow(fact[N-1], MOD-2);
    for (int i = N-1; i > 0; i--) ifact[i-1] = ifact[i] * i;
} } _fact_initialization;
inline static ModInt C(int x, int y) { if (x < y || x < 0 || y < 0) return 0; return fact[x] * ifact[y] * ifact[x - y]; }
inline static void umin(int &x, int y) { (x > y) && (x = y); }
inline static void umax(int &x, int y) { (x < y) && (x = y); }

static int64_t dis[N];
static int f[N][2], n, m, k;

namespace PntDivide {
    static bool ban[N];
    static int sz[N], mnSubSz, totSz, centroid;
    static void getCentroid(int u, int pre) {
        sz[u] = 1; int now = 0;
        for (auto [v, w] : g[u]) if (v != pre && !ban[v])
            getCentroid(v, u), sz[u] += sz[v], umax(now, sz[v]);
        umax(now, totSz - sz[u]);
        if (now < mnSubSz) mnSubSz = now, centroid = u;
    }
    using PntRec = std::vector<std::pair<int64_t,int>>;
    static PntRec allpnts;
    static struct __On_Init { __On_Init() { allpnts.reserve(N); } } __initialization;
    static void calcSzDis(int u, int pre) {
        sz[u] = 1; allpnts.emplace_back(dis[u], u);
        for (auto [v, w] : g[u]) if (v != pre && !ban[v])
            dis[v] = dis[u] + w, calcSzDis(v, u), sz[u] += sz[v];
    }
    static inline void makeCentroid(int &u, int totsz) {
        totSz = totsz, mnSubSz = totsz; getCentroid(u, 0); u = centroid;
        allpnts.clear(); dis[u] = 0; calcSzDis(u, 0);
    }
    bool upperBoundCmp(int x, const std::pair<int64_t,int>& pir) { return x < pir.first; }
    static void dfs(int u = 1, int totsz = n) {
        makeCentroid(u, totsz);
        // fprintf(stderr, "Centroid: %d\n", u);
        auto sortedall = allpnts;
        std::list<PntRec> all;
        int idx = 1;
        for (auto [v, w] : g[u]) if (!ban[v]) {
            std::sort(allpnts.data()+idx, allpnts.data()+idx+sz[v]);
            all.emplace_back(allpnts.data()+idx, allpnts.data()+idx+sz[v]);
            idx += sz[v];
        }
        std::sort(allpnts.begin(), allpnts.end());
        for (PntRec pnts : all) {
            for (const auto &[d, v] : pnts) {
                if (d > k) break;
                f[v][0] += (std::upper_bound(allpnts.begin(), allpnts.end(), k - d, upperBoundCmp) - allpnts.begin())
                         - (std::upper_bound(pnts.begin(), pnts.end(), k - d, upperBoundCmp) - pnts.begin());
            }
        }
        // fprintf(stderr, "Node %d: ", u);
        // for (auto [w, v] : allpnts) fprintf(stderr, "(%lld,%d)", w, v);
        // fprintf(stderr, " {+= %d}\n", int(std::upper_bound(allpnts.begin(), allpnts.end(), k, upperBoundCmp) - allpnts.begin()));
        f[u][0] += std::upper_bound(allpnts.begin(), allpnts.end(), k, upperBoundCmp) - allpnts.begin();
        ban[u] = true;
        for (auto [v, w] : g[u]) if (!ban[v]) dfs(v, sz[v]);
    }
    inline void init(int n) {
        memset(&f[1][0], 0, sizeof(ModInt) * 2*n);
        memset(PntDivide::ban + 1, 0, n);
    }
} // namespace PntDivide

namespace SegCombine {
    constexpr int64_t DOWN = 0, UP = 1e15;
    struct Node { int sum, ls, rs; };
    static std::vector<Node> tr(N * LGN);
    static std::stack<int> bin;
    static int tot;
    inline int alloc(int val = 0) {
        int u;
        if (bin.empty()) u = ++tot;
        else u = bin.top(), bin.pop();
        tr[u] = {val, 0, 0};
        return u;
    }
    inline void pushup(int u) {
        tr[u].sum = tr[tr[u].ls].sum + tr[tr[u].rs].sum;
    }
    int combine(int u, int v, int64_t l, int64_t r) {
        if (!u || !v) return u | v;
        tr[u].sum += tr[v].sum;
        if (l != r) {
            int64_t mid = l + r >> 1;
            tr[u].ls = combine(tr[u].ls, tr[v].ls, l, mid);
            tr[u].rs = combine(tr[u].rs, tr[v].rs, mid+1, r);
        }
        bin.push(v);
        return u;
    }
    struct SegTr {
        int root;
        int64_t L, R, P;
        int inq(int u, int64_t l, int64_t r) {
            if (!u) return 0;
            if (L <= l && r <= R) return tr[u].sum;
            int64_t mid = l + r >> 1;
            int res = 0;
            if (L <= mid) res += inq(tr[u].ls, l, mid);
            if (mid < R) res += inq(tr[u].rs, mid+1, r);
            return res;
        }
        int inquire(int64_t l, int64_t r) {
            L=l, R=r; return inq(root, DOWN, UP);
        }
        SegTr() : root() {}
        void upd(int& u, int64_t l, int64_t r) {
            if (!u) u = alloc();
            if (l == r) { tr[u].sum++; return; }
            int64_t mid = l + r >> 1;
            if (P <= mid) upd(tr[u].ls, l, mid);
            else upd(tr[u].rs, mid+1, r);
            pushup(u);
        }
        void update(int64_t pos) {
            P=pos; upd(root, DOWN, UP);
        }
        void merge(SegTr &oth) {
            root = combine(root, oth.root, DOWN, UP);
            oth.root = 0;
        }
    } fseg[N];
    inline void init(int n) {
        for (int i = 1; i <= n; i++) fseg[i].root = 0;
        tot = 0; std::stack<int>().swap(bin);
    }
    void dfs(int u = 1, int pre = 0) {
        // fprintf(stderr, "Dis[%d] = %lld\n", u, dis[u]);
        for (auto [v, w] : g[u]) if (v != pre) {
            dis[v] = dis[u] + w;
            dfs(v, u);
            if (pre) {
                f[u][1] += fseg[v].inquire(dis[pre]+k+1, dis[u]+k);
                // fprintf(stderr, "f[%d][1] += %d  (%lld,%lld]\n", u, fseg[v].inquire(dis[pre]+k+1, dis[u]+k), dis[pre]+k, dis[u]+k);
            }
            fseg[u].merge(fseg[v]);
        }
        fseg[u].update(dis[u]);
    }
} // namespace SegCombine
int main() {
    int tid, T;
    scanf("%d%d", &tid, &T);
    while (T--) {
        scanf("%d%d%d", &n, &m, &k);
        for (int i = 1; i <= n; i++) g[i].clear();
        for (int i = 1, u, v, w; i < n; i++) {
            scanf("%d%d%d", &u, &v, &w);
            g[u].emplace_back(v, w);
            g[v].emplace_back(u, w);
        }
        PntDivide::init(n); SegCombine::init(n);
        PntDivide::dfs(); SegCombine::dfs();
        ModInt ans = 0;
        f[1][1] = f[1][0];
        for (int i = 1; i <= n; i++) {
            ans += C(f[i][0], m) - C(f[i][0] - f[i][1], m);
            // fprintf(stderr, "%d %d +%d -%d %d\n", f[i][0], f[i][1], int(C(f[i][0], m)), int(C(f[i][0] - f[i][1], m)), int(ans));
        }
        printf("%d\n", int(ans));
    }
    return 0;
}