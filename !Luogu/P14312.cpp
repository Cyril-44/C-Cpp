#include <stdio.h>
#include <string.h>
#include <algorithm>
#ifdef DEBUG
# define decrypt(...) fprintf(stderr, __VA_ARGS__)
#else
# define decrypt(...) ((void)0)
#endif
#define umin(x, y) ((x) = std::min((x), (y)))
#define umax(x, y) ((x) = std::max((x), (y)))
constexpr int N = 150005, _K = 3, LOGN = 22;
int a[N];
int K;
using LL = long long;
namespace KDTree {
    int tot = 0;
    struct Node {
        LL w, sum; // 当前节点的权值、子树的权值和
        LL add; // 懒标记
        int cnt; // 当前子树有效节点数量
        int l, r; // 左儿子，右儿子
        LL dat[_K], mn[_K], mx[_K]; // 维度信息 和标记
        inline LL& operator[](int p) { return dat[p]; }
        inline LL operator[](int p) const { return dat[p]; }
        inline void pull(LL tag) { sum += tag * cnt, w += tag, add += tag; }
    } tr[N];
    int rt[LOGN]; // 建立
    inline void pushup(int u) {
        Node &us = tr[u];
        us.sum = us.w, us.cnt = 1;
        memcpy(us.mn, us.dat, sizeof us.dat);
        memcpy(us.mx, us.dat, sizeof us.dat);
        if (us.l) {
            const Node &ls = tr[us.l];
            us.sum += ls.sum, us.cnt += ls.cnt;
            for (int i = 0; i ^ K; ++i) // 更新维度信息
                umin(us.mn[i], ls.mn[i]),
                umax(us.mx[i], ls.mx[i]);
        }
        if (us.r) {
            const Node &rs = tr[us.r];
            us.sum += rs.sum, us.cnt += rs.cnt;
            for (int i = 0; i ^ K; ++i)
                umin(us.mn[i], rs.mn[i]),
                umax(us.mx[i], rs.mx[i]);
        }
    }
    inline void pushdown(int u) {
        if (tr[u].add) {
            if (tr[u].l) tr[tr[u].l].pull(tr[u].add);
            if (tr[u].r) tr[tr[u].r].pull(tr[u].add);
            tr[u].add = 0;
        }
    }
    void flatten(int &u) { // 将 u 这颗子树拍扁，拼接到 a 数组的末尾
        if (!u) return;
        a[++a[0]] = u;
        pushdown(u);
        flatten(tr[u].l);
        flatten(tr[u].r);
        u = 0;
    }
    int build(int l = 1, int r = a[0], int k = 0) {
        int mid = l + r >> 1;
        std::nth_element(a+l, a+mid, a+r+1, [k](int i, int j){ return tr[i][k] < tr[j][k]; });
        int u = a[mid];
        tr[u].l = (l ^ mid) ? build(l, mid - 1, (k+1) % K) : 0;
        tr[u].r = (mid ^ r) ? build(mid + 1, r, (k+1) % K) : 0;
        pushup(u);
        return u;
    }
    inline void insert(LL dat[], LL v) {
        ++tot;
        memcpy(tr[tot].dat, dat, sizeof tr[tot].dat);
        tr[tot].w = v;
        a[a[0] = 1] = tot;
        for (int i = 0; i < LOGN; i++) { // 如同二进制加法，每次加一
            if (!rt[i]) { rt[i] = build(); break; }
            else flatten(rt[i]);
        }
    }
    Node L, R; LL X;
    inline bool fullin(const Node &x) {
        for (int i = 0; i ^ K; ++i)
            if (!(L[i] <= x.mn[i] && x.mx[i] <= R[i])) return false;
        return true;
    }
    inline bool partin(const Node &x) {
        for (int i = 0; i ^ K; ++i)
            if (R[i] < x.mn[i] || x.mx[i] < L[i]) return false;
        return true;
    }
    inline bool selfin(const Node &x) {
        for (int i = 0; i ^ K; ++i)
            if (!(L[i] <= x[i] && x[i] <= R[i])) return false;
        return true;
    }
    LL que(int u) { // 查询 (L.x, L.y) 到 (R.x, R.y) 这个矩形内所有点的和
        if (!u) return 0;
        if (fullin(tr[u])) return tr[u].sum;
        if (!partin(tr[u])) return 0;
        LL ans = selfin(tr[u]) ? tr[u].w : 0;
        pushdown(u);
        return ans + que(tr[u].l) + que(tr[u].r);
    }
    inline LL query(LL l[], LL r[]) {
        memcpy(L.dat, l, sizeof L.dat);
        memcpy(R.dat, r, sizeof R.dat);
        LL ans = 0;
        for (int i = 0; i < LOGN; i++)
            ans += que(rt[i]);
        return ans;
    }
    void upd(int u) {
        if (!u) return;
        if (fullin(tr[u])) return tr[u].pull(X);
        if (!partin(tr[u])) return;
        if (selfin(tr[u])) tr[u].w += X;
        pushdown(u);
        upd(tr[u].l), upd(tr[u].r);
        pushup(u);
    }
    inline void update(LL l[], LL r[], LL x) {
        X = x;
        memcpy(L.dat, l, sizeof L.dat);
        memcpy(R.dat, r, sizeof R.dat);
        for (int i = 0; i < LOGN; i++)
            upd(rt[i]);
    }
}
int main() {
    int m;
    scanf("%d%d", &K, &m);
    LL x[_K], y[_K], v, last_ans = 0;
#define ReadArr(x) \
    for (int i = 0; i < K; i++) \
        scanf("%lld", &x[i]), x[i] ^= last_ans;
#define PrArr(x) \
    for (int i = 0; i < K; i++) \
        decrypt("%lld ", x[i]);
    for (int op; m--; ) {
        scanf("%d", &op);
        decrypt("%d ", op);
        if (op == 1) {
            ReadArr(x);
            scanf("%lld", &v); v ^= last_ans;
            PrArr(x); decrypt("%lld\n", v);
            KDTree::insert(x, v);
        } else if (op == 2) {
            ReadArr(x); ReadArr(y);
            scanf("%lld", &v); v ^= last_ans;
            PrArr(x); PrArr(y); decrypt("%lld\n", v);
            KDTree::update(x, y, v);
        } else {
            ReadArr(x); ReadArr(y);
            PrArr(x); PrArr(y); decrypt("\n");
            printf("%lld\n", last_ans = KDTree::query(x, y));            
        }
    }
    return 0;
}