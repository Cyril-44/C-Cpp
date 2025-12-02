#include <stdio.h>
#include <string.h>
#include <algorithm>
#define umin(x, y) ((x) = std::min((x), (y)))
#define umax(x, y) ((x) = std::max((x), (y)))
constexpr int N = 200005, K = 2, LOGN = 22;
int a[N];
namespace KDTree {
    int tot = 0;
    struct Node {
        int w, sum; // 当前节点的权值、子树的权值和
        int l, r; // 左儿子，右儿子
        int dat[K], mn[K], mx[K]; // 维度信息 和标记
        inline int& operator[](int p) { return dat[p]; }
        inline int operator[](int p) const { return dat[p]; }
    } tr[N];
    int rt[LOGN]; // 建立
    inline void pushup(int u) {
        Node &us = tr[u];
        us.sum = us.w;
        memcpy(us.mn, us.dat, sizeof(int) * K);
        memcpy(us.mx, us.dat, sizeof(int) * K);
        if (us.l) {
            const Node &ls = tr[us.l];
            us.sum += ls.sum;
            for (int i = 0; i ^ K; ++i) // 更新维度信息
                umin(us.mn[i], ls.mn[i]),
                umax(us.mx[i], ls.mx[i]);
        }
        if (us.r) {
            const Node &rs = tr[us.r];
            us.sum += rs.sum;
            for (int i = 0; i ^ K; ++i)
                umin(us.mn[i], rs.mn[i]),
                umax(us.mx[i], rs.mx[i]);
        }
    }
    void flatten(int &u) { // 将 u 这颗子树拍扁，拼接到 a 数组的末尾
        if (!u) return;
        a[++a[0]] = u;
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
    inline void insert(int x, int y, int v) {
        ++tot;
        tr[tot][0] = x, tr[tot][1] = y, tr[tot].w = v;
        a[a[0] = 1] = tot;
        for (int i = 0; i < LOGN; i++) { // 如同二进制加法，每次加一
            if (!rt[i]) { rt[i] = build(); break; }
            else flatten(rt[i]);
        }
    }
    Node L, R;
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
    int que(int u) { // 查询 (L.x, L.y) 到 (R.x, R.y) 这个矩形内所有点的和
        if (!u) return 0;
        if (fullin(tr[u])) return tr[u].sum;
        if (!partin(tr[u])) return 0;
        int ans = selfin(tr[u]) ? tr[u].w : 0;
        return ans + que(tr[u].l) + que(tr[u].r);
    }
    inline int query(int x1, int y1, int x2, int y2) {
        L[0] = x1, L[1] = y1;
        R[0] = x2, R[1] = y2;
        int ans = 0;
        for (int i = 0; i < LOGN; i++)
            ans += que(rt[i]);
        return ans;
    }
}
int main() {
#ifdef DEBUG
    freopen("P4148.in", "r", stdin);
#endif
    int n;
    int last_ans = 0;
    scanf("%d", &n);
    for (int op; ; ) {
        scanf("%d", &op);
        if (op == 3) break;
        if (op == 1) {
            int x, y, A;
            int __cnt = scanf("%d%d%d", &x, &y, &A);
            x ^= last_ans, y ^= last_ans, A ^= last_ans;
            KDTree::insert(x, y, A);
        } else {
            int x1, y1, x2, y2;
            int __cnt = scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
            x1 ^= last_ans, y1 ^= last_ans, x2 ^= last_ans, y2 ^= last_ans;
            printf("%d\n", last_ans = KDTree::query(x1, y1, x2, y2));
        }
    }
    return 0;
}