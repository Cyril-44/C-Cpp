#include <cstdio>
#include <cstdlib>
#include <queue>
#include <algorithm>
#include <vector>
// #define debug(...) fprintf(stderr, __VA_ARGS__)
#define debug(...) 114514
const int N = 100005;
struct Node {
    unsigned dfn, edfn;
    int val, size, hson, top, dep, fa;
    std::vector<int> sons;
} a[N];
inline void dfs1(const int &u, const int &fa) {
    a[u].fa = fa;
    a[u].size = 1;
    int mxv = -1;
    for (const int &v : a[u].sons) {
        if (v == fa) continue;
        dfs1(v, u);
        a[u].size += a[v].size;
        if (mxv == -1 || a[v].size > a[mxv].size) {
            mxv = v;
        }
    }
    debug("dfs1: %d %d\n", u, mxv);
    a[u].hson = mxv;
}
unsigned time_stamp;
inline void dfs2(const int &u, const int &fa) {
    debug("dfs2: %d %d %d\n", u, a[u].hson, fa);
    a[u].dfn = ++time_stamp;
    if (~a[u].hson) {
        a[a[u].hson].top = a[u].top;
        a[a[u].hson].dep = a[u].dep + 1;
        dfs2(a[u].hson, u);
    }
    for (const int &v : a[u].sons) {
        if (v == fa || v == a[u].hson) continue;
        a[v].top = v;
        a[v].dep = a[u].dep + 1;
        dfs2(v, u);
    }
    a[u].edfn = time_stamp;
}
int n, m, r, P=1;
struct ModInt {
    int num;
    ModInt(int x=0) {
        num = x % P;
        if (num < 0) num += P;
    }
    ModInt operator-() const {
        return ModInt(-num);
    }
    ModInt& operator+=(const ModInt &x) {
        num = (num + x.num) % P;
        if (num < 0) num += P;
        return *this;
    }
    ModInt operator+(const ModInt &x) const {
        ModInt res = *this;
        res += x;
        return res;
    }
    ModInt& operator-=(const ModInt &x) {
        num = (num - x.num) % P;
        if (num < 0) num += P;
        return *this;
    }
    ModInt operator-(const ModInt &x) const {
        ModInt res = *this;
        res -= x;
        return res;
    }
    ModInt& operator*=(const ModInt &x) {
        num = (1LL * num * x.num) % P;
        if (num < 0) num += P;
        return *this;
    }
    ModInt operator*(const ModInt &x) const {
        ModInt res = *this;
        res *= x;
        return res;
    }
};
struct SegNode {
    ModInt sum, lazy;
} tr[N << 2];
inline void pushup(const int &u) {
    tr[u].sum = tr[u<<1].sum + tr[u<<1|1].sum;
}
inline void pushdown(const int &u, const int &range_l, const int &range_r) {
    if (tr[u].lazy.num) {
        tr[u<<1].lazy += tr[u].lazy;
        tr[u<<1].sum += tr[u].lazy * range_l;
        tr[u<<1|1].lazy += tr[u].lazy;
        tr[u<<1|1].sum += tr[u].lazy * range_r;
        tr[u].lazy = 0;
    }
}
inline void upd(const ModInt &X, const unsigned &L, const unsigned &R, int u=1, unsigned l=1, unsigned r=n) {
    if (L <= l && r <= R) {
        tr[u].sum += X * (r - l + 1);
        tr[u].lazy += X;
        return;
    }
    unsigned mid = l + r >> 1;
    int l_len = mid - l + 1;
    int r_len = r - mid;
    pushdown(u, l_len, r_len);
    if (L <= mid) upd(X, L, R, u<<1, l, mid);
    if (R > mid) upd(X, L, R, u<<1|1, mid+1, r);
    pushup(u);
}
inline ModInt query(const unsigned &L, const unsigned &R, int u=1, unsigned l=1, unsigned r=n) {
    if (L <= l && r <= R)
        return tr[u].sum;
    unsigned mid = l + r >> 1;
    int l_len = mid - l + 1;
    int r_len = r - mid;
    pushdown(u, l_len, r_len);
    ModInt ret(0);
    if (L <= mid) ret = ret + query(L, R, u<<1, l, mid);
    if (R > mid) ret = ret + query(L, R, u<<1|1, mid+1, r);
    return ret;
}
using std::swap;
inline void add(int x, int y, const int &z) {
    ModInt Z(z);
    while (a[x].top != a[y].top) {
        if (a[a[x].top].dep < a[a[y].top].dep) swap(x, y);
        upd(Z, a[a[x].top].dfn, a[x].dfn);
        x = a[a[x].top].fa;
    }
    if (a[x].dep < a[y].dep) swap(x, y);
    upd(Z, a[y].dfn, a[x].dfn);
}
inline int sum(int x, int y) {
    ModInt ret;
    while (a[x].top != a[y].top) {
        if (a[a[x].top].dep < a[a[y].top].dep) swap(x, y);
        ret = ret + query(a[a[x].top].dfn, a[x].dfn);
        x = a[a[x].top].fa;
    }
    if (a[x].dep < a[y].dep) swap(x, y);
    ret = ret + query(a[y].dfn, a[x].dfn);
    return ret.num;
}
int main() {
    freopen("P3384_4.in", "r", stdin);
    scanf("%d%d%d%d", &n, &m, &r, &P);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i].val);
    for (int i = 1; i < n; i++) {
        int u, v;
        scanf("%d%d", &u, &v);
        a[u].sons.emplace_back(v);
        a[v].sons.emplace_back(u);
    }
    dfs1(r, 0);
    a[r].dep = 0;
    a[r].top = r;
    dfs2(r, 0);
    for (int i = 1; i <= n; i++)
        upd(ModInt(a[i].val), a[i].dfn, a[i].dfn);
    for (int opt, x, y, z; m--; ) {
        scanf("%d%d", &opt, &x);
        switch (opt) {
        case 1:
            scanf("%d%d", &y, &z);
            add(x, y, z);
            break;
        case 2:
            scanf("%d", &y);
            printf("%d\n", sum(x, y));
            break;
        case 3:
            scanf("%d", &z);
            upd(ModInt(z), a[x].dfn, a[x].edfn);
            break;
        case 4:
            printf("%d\n", query(a[x].dfn, a[x].edfn).num);
        }
    }
    return 0;
}