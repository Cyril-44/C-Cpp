#include <stdio.h>
#include <array>
#include <vector>
#include <algorithm>
constexpr int N = 150005, K = 2, MOD = 998244353;
#define umin(x, y) ((x) < (y) ? (x) : (x) = (y))
#define umax(x, y) ((y) < (x) ? (x) : (x) = (y))
using HyPoint = std::array<int, K>;
HyPoint a[N];
int hist[N];
enum Type {ADD, PROD, QUE};
struct Query {
    Type type;
    int l, r, s, t, d;
};
namespace KDT {
inline void Add(int& x, int y) { if ((x += y) >= MOD) x -= MOD; }
inline void Mul(int &x, int y) { x = 1ull * x * y % MOD; }
struct Node {
    HyPoint v, l, r;
    int ls, rs;
    int val, add, mul;
    Node() : mul(1) {}
    inline void opAdd(int x) {
        if (!x) return;
        Add(val, x);
        Add(add, x);
    }
    inline void opMul(int x) {
        if (x == 1) return;
        Mul(val, x);
        Mul(add, x);
        Mul(mul, x);
    }
} tr[N];
using Iter = std::vector<HyPoint>::iterator;
int tot;
inline void pushup(int u) {
    tr[u].l = tr[u].r = tr[u].v;
    if (tr[u].ls) {
        auto& tls = tr[tr[u].ls];

        for (int k = 0; k ^ K; k++) {
            umin(tr[u].l[k], tls.l[k]);
            umax(tr[u].r[k], tls.r[k]);
        }
    }
    if (tr[u].rs) {
        auto& trs = tr[tr[u].rs];
        for (int k = 0; k ^ K; k++) {
            umin(tr[u].l[k], trs.l[k]);
            umax(tr[u].r[k], trs.r[k]);
        }
    }
}
inline void pushdown(int u) {
    if (tr[u].ls) {
        auto &tls = tr[tr[u].ls];
        tls.opMul(tr[u].mul);
        tls.opAdd(tr[u].add);
    }
    if (tr[u].rs) {
        auto &trs = tr[tr[u].rs];
        trs.opMul(tr[u].mul);
        trs.opAdd(tr[u].add);
    }
    tr[u].add = 0, tr[u].mul = 1;
}
int rt;
void build(Iter beg, Iter end, int k = 0, int &u = rt) {
    if (beg == end) return;
    u = ++tot;
    Iter mid = beg + (end - beg >> 1);
    std::nth_element(beg, mid, end, [k](const auto &x, const auto &y) {
        return x[k] < y[k];
    });
    tr[u].v = *mid;
    build(beg, mid, (k+1) % K, tr[u].ls);
    build(mid+1, end, (k+1) % K, tr[u].rs);
    pushup(u);
}
HyPoint L, R;
int Ad, Mu;
inline bool selfin(int u) {
    for (int k = 0; k ^ K; ++k)
        if (tr[u].v[k] < L[k] || tr[u].v[k] > R[k]) return false;
    return true;
}
inline bool partin(int u) {
    for (int k = 0; k ^ K; ++k)
        if (tr[u].r[k] < L[k] || tr[u].l[k] > R[k]) return false;
    return true;
}
inline bool fullin(int u) {
    for (int k = 0; k ^ K; ++k)
        if (tr[u].l[k] < L[k] || tr[u].r[k] > R[k]) return false;
    return true;
}
void upd(int u) {
    if (!u || !partin(u)) return;
    if (fullin(u)) {
        tr[u].opMul(Mu);
        tr[u].opAdd(Ad);
        return;
    }
    if (selfin(u)) {
        Mul(tr[u].val, Mu);
        Add(tr[u].val, Ad);
    }
    pushdown(u);
    upd(tr[u].ls), upd(tr[u].rs);
    return;
}
inline void modify(HyPoint &&l, HyPoint &&r, int x, int y) {
    L = l, R = r, Ad = x, Mu = y;
    upd(rt);
}
HyPoint P;
inline int que(int u) {
    if (!u || !partin(u)) return 0;
    if (tr[u].v == P) return tr[u].val;
    pushdown(u);
    return que(tr[u].ls) | que(tr[u].rs);
}
inline int query(HyPoint &&p) {
    L = R = P = p;
    return que(rt);
}
inline void debug_all() {
    for (int i = 1; i <= tot; i++) {
        pushdown(i);
        printf("(%d,%d): val=%d, add=%d, mul=%d, rg=([%d,%d],[%d,%d])\n", 
            tr[i].v[0], tr[i].v[1], tr[i].val, tr[i].add, tr[i].mul, tr[i].l[0], tr[i].r[0], tr[i].l[1], tr[i].r[1]);
    }
}
}
int main() {
    int n, m;
    std::vector<Query> que;
    std::vector<HyPoint> pnt;
    Initialization: {
        scanf("%d%d", &n, &m);
        que.reserve(m);
        for (int i = 1, op, l, r, d; i <= m; i++) {
            scanf("%d%d", &op, &l);
            if (op <= 2) scanf("%d%d", &r, &d), d %= MOD;
            switch (op) {
            case 1: que.push_back({ADD , l, r, i, m, d}); break;
            case 2: que.push_back({PROD, l, r, i, m, d}); break;
            case 3: que.push_back({QUE , l, l, i, i, 0});
                    pnt.push_back({l, i});                break;
            case 4: l = hist[l] - 1; que[l].t = i;        break;
            }
            hist[i] = que.size();
        }
        KDT::build(pnt.begin(), pnt.end());
    }
    Solving: {
        for (const auto &q : que) {
            switch (q.type) {
            case ADD:  KDT::modify({q.l, q.s}, {q.r, q.t}, q.d, 1); break;
            case PROD: KDT::modify({q.l, q.s}, {q.r, q.t}, 0, q.d); break;
            case QUE:  printf("%d\n", KDT::query({q.l, q.t}));      break;
            }
            /* printf("%s: x[%d, %d], t[%d, %d], %d\n", [](Type tp) {
                switch (tp) {
                case ADD: return "ADD";
                case PROD: return "PROD";
                case QUE: return "QUE";
                }
                return "Fuck you";
            }(q.type), q.l, q.r, q.s, q.t, q.d);
            KDT::debug_all(); */
        }
    }
    return 0;
}