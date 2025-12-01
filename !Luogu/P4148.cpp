#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <algorithm>
#include <unordered_map>
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
    } tr[N];
    int rt[LOGN]; // 建立
    inline void pushup(int u) {
        Node &us = tr[u];
        us.sum = us.w;
        memcpy(us.mn, us.dat, sizeof us.dat);
        memcpy(us.mx, us.dat, sizeof us.dat);
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
        a[++a[0]] = tr[u].w;
        flatten(tr[u].l);
        flatten(tr[u].r);
        u = 0;
    }
    int build(int l = 1, int r = a[0], int k = 0) {
        int mid = l + r >> 1;
        std::nth_element(a+l, a+mid, a+r+1, [k](int i, int j){ return tr[i][k] < tr[j][k]; })
        int u = a[mid];
        tr[u].l = (l ^ mid) ? build(l, mid - 1, (k+1) % K) : 0;
        tr[u].r = (mid ^ r) ? build(mid + 1, r, (k+1) % K) : 0;
        return u;
    }
    inline void insert(int x) {
         
    }
}
int main() {
    int last_ans = 0;
    scanf("%*d");
    for (char op; scanf(" %c", &op) && (op ^ '3'); ) {
        if (op == 1) {
            int x, y, A;
            scanf("%d%d%d", &x, &y, &A);
        } else {
            int x1, y1, x2, y2;
            scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
        }
    }
    return 0;
}