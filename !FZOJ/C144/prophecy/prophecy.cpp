#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
#define umax(x, y) ((x) = std::max((x), (y)))
constexpr int N = 500005;
int a[N], sta1[N], sta2[N];
std::vector<int> chks[N];
std::vector<std::pair<int,int>> ques[N];
int n, ans[N];
namespace SegTr {
    int tr[N << 2], lazy[N << 2]; // 记录 max{a+b+c}
    int mx[N << 2]; // 辅助，记录 c 在线段树上每一小段的最大值
    void build(int u=1, int l=1, int r=n) {
        if (l == r) tr[u] = mx[u] = a[l];
        else {
            int mid = l + r >> 1;
            build(u << 1, l, mid);
            build(u << 1 | 1, mid + 1, r);
            tr[u] = mx[u] = std::max(mx[u<<1], mx[u<<1|1]);
        }
    }
    inline void pushdown(int u) {
        if (lazy[u]) {
            umax(tr[u<<1], lazy[u] + mx[u<<1]);
            umax(tr[u<<1|1], lazy[u] + mx[u<<1|1]);
            umax(lazy[u<<1], lazy[u]);
            umax(lazy[u<<1|1], lazy[u]);
            lazy[u] = 0;
        }
    }
    inline int query(int L, int R, int u=1, int l=1, int r=n) {
        if (L <= l && r <= R) return tr[u];
        int mid = l + r >> 1, res = 0;
        pushdown(u);
        if (L <= mid) umax(res, query(L, R, u<<1, l, mid));
        if (mid < R) umax(res, query(L, R, u<<1|1, mid+1, r));
        return res;
    }
    inline void upd(int L, int R, int X, int u=1, int l=1, int r=n) {
        if (L <= l && r <= R) {
            umax(tr[u], mx[u] + X);
            umax(lazy[u], X);
            return;
        }
        int mid = l + r >> 1;
        pushdown(u);
        if (L <= mid) upd(L, R, X, u<<1, l, mid);
        if (mid < R) upd(L, R, X, u<<1|1, mid+1, r);
        tr[u] = std::max(tr[u<<1], tr[u<<1|1]);
    }
}
int main() {
    freopen("prophecy.in", "r", stdin);
    freopen("prophecy.out", "w", stdout);
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    // int top1=0, top2=0; // a[sta_i] 栈顶最小 a[q2_i] 栈顶最大
    // for (int i = 1; i <= n; i++) {
    //     while (top1 && a[i] > a[sta1[top1]]) --top1;
    //     while (top2 && a[i] < a[sta2[top2]]) --top2;
    //     if (top1) chks[sta1[top1]].push_back(i);
    //     if (top2) chks[sta2[top2]].push_back(i);
    //     sta1[++top1] = i, sta2[++top2] = i;
    // }
    {
        static int sta[N];
        int tot = 0;
        for(int i = 1; i <= n; ++i){
            while(a[sta[tot]] < a[i] && tot) --tot;
            if(tot) chks[sta[tot]].push_back(i);
            sta[++tot] = i;
        }
        tot = 0;
        for(int i = n; i; --i){
            while(a[sta[tot]] < a[i] && tot) --tot;
            if(tot) chks[i].push_back(sta[tot]);
            sta[++tot] = i;
        }
    }
    int m;
    scanf("%d",&m);
    for (int i = 1, l, r; i <= m; i++) {
        scanf("%d%d", &l, &r);
        ques[l].emplace_back(r, i);
    }
    SegTr::build();
    for (int i = n, k; i >= 1; i--) { // 保证当前 SegTr 中最小的 a 为 i
        for (int j : chks[i])
            if ((k = i+(j-i)*2) <= n)
                SegTr::upd(k, n, a[i] + a[j]);
        for (const auto &q : ques[i]) {
            if (q.second == 8) {
                for (int k = q.first; k <= n; k++)
                    if (SegTr::query(i, k) >= 208)  
                        fprintf(stderr, "%d %d %d\n", i, k, SegTr::query(i, k));
            }
            ans[q.second] = SegTr::query(i, q.first);
        }
    }
    for (int i = 1; i <= m; i++)
        printf("%d\n", ans[i]);
    return 0;
}