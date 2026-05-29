#include <cstdio>
#include <vector>
#include <algorithm>
#include <bitset>
#include <list>
constexpr int N = 1/*00005*/, LGN = 17, MOD = (int)1e9 + 9;
std::vector<std::pair<int,int>> g[N];
int n, m, k;
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
} f[N][2];
namespace BruteForce {
constexpr int N = 10;
std::bitset<N> curst, all[N];
ModInt C[N][N];
void getall(int u, int pre, int dep) {
    if (dep > k) return;
    curst.set(u);
    for (auto [v, w] : g[u]) if (v != pre) getall(v, u, dep + w);
}
inline void work() {
    for (int i = 1; i <= n; i++)
        curst.reset(), getall(i, 0, 0), all[i] = curst;
    for (int i = 1; i <= n; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++) C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    ModInt ans = 0;
    std::vector<unsigned> maskall[N];
    for (unsigned mask = 0; !(mask>>n); mask++)
        maskall[__builtin_popcount(mask)].push_back(mask << 1);
    for (int i = 1; i <= n; i++) {
        for (unsigned mask : maskall[i]) {
            std::bitset<N> st; st.set();
            for (int j = 1; j <= n; j++)
                if (mask >> j & 1) st &= all[j];
            int sz = st.count();
            if (i & 1) ans += C[sz][m];
            else ans -= C[sz][m];
        }
    }
    printf("%d\n", int(ans));
}
} // namespace Bruteforce
namespace PntDivide {
    std::bitset<N> ban;
    int sz[N], mnSubSz, totSz, centroid;
    int64_t dis[N];
    void getCentroid(int u, int pre) {
        sz[u] = 1;
        int now = 0;
        for (auto [v, w] : g[u]) if (v != pre && !ban[v]) {
            getCentroid(v, u);
            sz[u] += sz[v];
            now = std::min(now, sz[v]);
        }
        now = std::max(now, totSz - sz[u]);
        if (now < mnSubSz) mnSubSz = now, centroid = u;
    }
    using PntRec = std::vector<std::pair<int64_t,int>>;
    PntRec allpnts;
    struct __On_Init { __On_Init() { allpnts.reserve(N); } } __initialization;
    void calcSzDis(int u, int pre) {
        sz[u] = 1; allpnts.emplace_back(dis[u], u);
        for (auto [v, w] : g[u]) if (v != pre && !ban[v]) {
            dis[v] = dis[u] + w;
            calcSzDis(v, u);
            sz[u] += sz[v];
        }
    }
    inline void makeCentroid(int &u, int sz) {
        totSz = sz, mnSubSz = sz;
        getCentroid(u, 0);
        u = centroid;
        allpnts.clear();
        dis[u] = 0;v
        calcSzDis(u, 0);
    }
    bool upperBoundCmp(int x, const std::pair<int64_t,int>& pir) { return x < pir.first; }
    void dfs(int u, int totsz) {
        makeCentroid(u, totsz);
        auto sortedall = allpnts;
        std::list<PntRec> all;
        int idx = 1;
        for (auto [v, w] : g[u]) if (!ban[u]) {
            std::sort(allpnts.data()+idx, allpnts.data()+idx+sz[v]);
            all.emplace_back(allpnts.data()+idx, allpnts.data()+idx+sz[v]);
            std::inplace_merge(allpnts.data(), allpnts.data()+idx, allpnts.data()+idx+sz[v]);
        }
        for (PntRec pnts : all) {
            for (const auto &[d, v] : pnts) {
                if (d > k) break;
                f[v][0] += std::max(0, int(std::upper_bound(allpnts.begin(), allpnts.end(), k - d, upperBoundCmp) - allpnts.begin() - 1))
                         - std::max(0, int(std::upper_bound(pnts.begin(), pnts.end(), k - d, upperBoundCmp) - pnts.begin() - 1));
            }
        }
        f[u][0] += std::max(0, int(std::upper_bound(allpnts.begin(), allpnts.end(), k-d, upperBoundCmp) - allpnts.begin() - 1));
        ban[u] = true;
        for (auto [v, w] : g[u]) if (!ban[u]) dfs(v, sz[v]);
    }
} // namespace PntDivide
namespace SegCombine {
    constexpr int DOWN = 0, UP = 1e9;
    struct Node { int sum, ls, rs; };
    static Node tr[N * LGN];
    int tot;
    inline int alloc(int val = 0) {
        tr[++tot] = {val};
        return tot;
    }
    inline void pushup(int u) {
        tr[u].sum = tr[tr[u].ls].sum + tr[tr[u].rs].sum;
    }
    int combine(int u, int v, int l, int r) {
        if (!u || !v) return u | v;
        tr[u].sum += tr[v].sum;
        if (l != r) {
            int mid = l + r >> 1;
            tr[u].ls = combine(tr[u].ls, tr[v].ls, l, mid);
            tr[u].rs = combine(tr[u].rs, tr[v].rs, mid+1, r);
        }
        return u;
    }
    struct SegTr {
        int root, L, R, P;
        int inq(int u, int l, int r) {
            if (!u) return 0;
            if (L <= l && r <= R) return tr[u].sum;
            int mid = l + r >> 1, res = 0;
            if (L <= mid) res += inq(tr[u].ls, l, mid);
            if (mid < R) res += inq(tr[u].rs, mid+1, r);
            return res;
        }
        int inquire(int l, int r) {
            L=l, R=r; return inq(root, DOWN, UP);
        }
        SegTr() : root() {}
        void upd(int& u, int l, int r) {
            if (!u) u = alloc();
            if (l == r) tr[u].sum++;
            int mid = l + r >> 1;
            if (P <= mid) upd(tr[u].ls, l, mid);
            else upd(tr[u].rs, mid+1, r);
            pushup(u);
        }
        void update(int pos) {
            P=pos; upd(root, DOWN, UP);
        }
        void merge(SegTr &oth) {
            root = combine(root, oth.root, DOWN, UP);
            oth.root = 0;
        }
    } fseg[N];
    void dfs(int u, int pre = 0) {
        for (auto [v, w] : g[u]) if (v != pre) {
            dfs(v, u);
            fseg[u].merge(fseg[v]);
        }
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
        
    }
    return 0;
}