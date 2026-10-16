#include <bits/stdc++.h>
constexpr int N = 100005, MOD = 998244353;
#ifdef CLANGD
#define N 15
#endif
struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    Mint pow(int n) {
        Mint b = *this, res = 1;
        while (n) {
            if (n & 1) res *= b;
            b *= b, n >>= 1;
        }
        return res;
    }
    bool operator==(const Mint o) const { return val == o.val; }
    Mint(int v=0) : val(v) {}
    template<typename T> explicit operator T() const { return static_cast<T>(val); }
    bool operator<(const Mint o) const { return val < o.val; }
private: int val;
};
std::vector<int> ids[64];
std::vector<Mint> a[N];
Mint curr[N], vali[N];
class SegTr {
    struct Node {
        std::valarray<Mint> ring;
        int lazy, ls, rs;
        void rotate(int t) { lazy += t; ring = ring.cshift(t); }
    } tr[N << 1];
    int rt[64], top, Sz, L, R;
    void pushup(int u) { tr[u].ring = tr[tr[u].ls].ring + tr[tr[u].rs].ring; }
    void pushdown(int u) {
        if (tr[u].lazy) {
            tr[tr[u].ls].rotate(tr[u].lazy);
            tr[tr[u].rs].rotate(tr[u].lazy);
            tr[u].lazy = 0;
        }
    }
    int build(int l, int r) {
        int u = ++top;
        tr[u].ring.resize(Sz);
        if (l != r) {
            int mid = l + r >> 1;
            tr[u].ls = build(l, mid);
            tr[u].rs = build(mid+1, r);
            pushup(u);
        }
        return u;
    }
    void rot(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].rotate(1);
        int mid = l + r >> 1;
        pushdown(u);
        if (L <= mid) rot(tr[u].ls, l, mid);
        if (mid < R) rot(tr[u].rs, mid+1, r);
        pushup(u);
    }
    Mint inq(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].ring[0];
        int mid = l + r >> 1; Mint res;
        pushdown(u);
        if (L <= mid) res += inq(tr[u].ls, l, mid);
        if (mid < R) res += inq(tr[u].rs, mid+1, r);
        return res;
    }
    void ext(int u, int l, int r) {
        if (l == r) curr[ids[Sz][l]] = tr[u].ring[0];
        else {
            int mid = l + r >> 1;
            pushdown(u);
            ext(tr[u].ls, l, mid);
            ext(tr[u].rs, mid+1, r);
        }
    }
    void add(int u, int l, int r) {
        if (l == r) {
            for (int i = 0; i < Sz; i++)
                tr[u].ring[i] = a[ids[Sz][l]][i];
            return;
        }
        int mid = l + r >> 1;
        pushdown(u);
        if (L <= mid) add(tr[u].ls, l, mid);
        else add(tr[u].rs, mid+1, r);
        pushup(u);
    }
public:
    void extract(int id) { Sz=id; ext(rt[id], 0, ids[id].size() - 1); }
    void build(int id) { Sz=id, rt[id] = build(0, ids[id].size() - 1); }
    void update(int id, int l, int r) { Sz=id, L=l, R=r, rot(rt[id], 0, ids[id].size() - 1); }
    Mint inquire(int id, int l, int r) { Sz=id, L=l, R=r; return inq(rt[id], 0, ids[id].size() - 1); }
    void add(int id, int p) { Sz=id, L=p; add(rt[id], 0, ids[id].size() - 1); }
} fseg;
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    std::set<std::tuple<int,Mint,int, int, int>> st;
    for (int i = 1; i <= n; i++) {
        Mint x; scanf("%d", &x);
        x = x.pow(MOD - 2);
        vali[i] = x;
        std::vector<Mint> arr;
        int endp = -1;
        for (Mint tp = x;; tp *= tp) {
            for (int i = 0; i < (int)arr.size(); i++)
                if (tp == arr[i]) { endp = i; break; }
            if (~endp) break;
            arr.push_back(tp);
        }
        a[i] = std::vector<Mint>(arr.begin() + endp, arr.end());
        st.emplace(i, x, endp, int(arr.size() - endp), (int)ids[arr.size() - endp].size());
        ids[arr.size() - endp].push_back(i);
    }
    for (int i = 1; i < 64; i++) if (!ids[i].empty()) fseg.build(i);
    for (int op, l, r, q1=0,q0=0; m--;) {
        scanf("%d%d%d", &op, &l, &r);
        Mint ans;
        auto itl = st.lower_bound({l,0,0,0,0}), itr = st.upper_bound({r,MOD-1,0,0,0});
        for (auto it = itl; it != itr; ) {
            auto [i, v, x, y, z] = *it;
            if (op == 0) {
                it = st.erase(it);
                if (!x) fseg.add(y, z);
                else st.emplace(i, v*v, x-1, y, z);
            }
            else ans += v, ++it;
        }
        for (int i = 1; i < 64; i++) if (!ids[i].empty()) {
            int il = int(std::lower_bound(ids[i].begin(), ids[i].end(), l) - ids[i].begin()),
                ir = int(std::upper_bound(ids[i].begin(), ids[i].end(), r) - ids[i].begin()) - 1;
            if (il <= ir) {
                if (op == 0) fseg.update(i, il, ir);
                else ans += fseg.inquire(i, il, ir);
            }
        }
        if (op == 1) //{
            printf("%d\n", ans);
            /* ++q1;
            Mint real = 0;
            for (int i = l; i <= r; i++)
                real += vali[i];
            assert(int(real) == int(ans));
        } else {
            ++q0;
            for (int i = l; i <= r; i++) vali[i] *= vali[i];
            for (int i = 1; i < 64; i++) if (!ids[i].empty()) fseg.extract(i);
            for (int i = 1; i <= n; i++) {
                auto it = st.lower_bound({i,0,0,0,0});
                if (it != st.end() && std::get<0>(*it) == i) curr[i] = std::get<1>(*it);
                assert(int(curr[i]) == int(vali[i]));
            }
        } */
    }
}