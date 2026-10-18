#include <bits/stdc++.h>
constexpr int N = 500005;
#ifdef CLANGD
#define N 55
#endif
std::pair<int,int> seg[N];
std::tuple<int,int,int> qry[N];
int n;
bool ans[N];
class SegTr {
    struct Node {
        int l = (int)1e9, r = (int)1e9;
        Node& operator+=(const Node& o) {
            if (o.l <= r) l = std::min(l, o.l), r = o.r;
            return *this;
        }
        friend Node operator+(Node x, Node y) { return x += y; }
    } tr[N << 2];
    int L, R; Node X;
    void pushdown(int u) {
        tr[u<<1] += tr[u];
        tr[u<<1|1] += tr[u];
        tr[u] = Node{};
    }
    void upd(int u, int l, int r) {
        if (L <= l && r <= R) { tr[u] += X; return; }
        int mid = l + r >> 1;
        pushdown(u);
        if (L <= mid) upd(u<<1, l, mid);
        if (mid < R) upd(u<<1|1, mid+1, r);
    }
    Node inq(int u, int l, int r) {
        if (l == r) return tr[u];
        int mid = l + r >> 1;
        pushdown(u);
        return L <= mid ? inq(u<<1, l, mid) : inq(u<<1|1, mid+1, r);
    }
public:
    void build(int u=1, int l=1, int r=n) {
        if (l == r) { tr[u] = {l-1, l-1}; return; }
        int mid = l + r >> 1;
        build(u<<1, l, mid), build(u<<1|1, mid+1, r);
    }
    void update(int l, int r, std::pair<int,int> q) { L=l, R=r, X = {q.first - 1, q.second}; upd(1, 1, n); }
    int inquire(int p) { L=p; return inq(1, 1, n).r; }
} f;
int main() {
    int m, q;
    scanf("%d%d%d", &n, &m, &q);
    for (int i = 1; i <= m; i++)
        scanf("%d%d", &seg[i].first, &seg[i].second);
    for (int i = 1, l, r; i <= q; i++) {
        scanf("%d%d", &l, &r);
        qry[i] = {l, r, i};
    }
    auto cmp = [](const auto& x, const auto& y) { return std::get<1>(x) < std::get<1>(y); };
    std::sort(seg+1, seg+1+m, cmp);
    std::sort(qry+1, qry+1+q, cmp);
    int idx = 1;
    f.build();
    for (int i = 1; i <= q; i++) {
        for (; idx <= m && seg[idx].second <= std::get<1>(qry[i]); idx++) {
            f.update(1, seg[idx].first, seg[idx]);
            // fprintf(stderr, "After add [%d, %d]: ", seg[idx].first, seg[idx].second);
            // for (int j = 1; j <= n; j++)
            //     fprintf(stderr, "%d%c", f.inquire(j), " \n"[j==n]);
        }
        ans[std::get<2>(qry[i])] = f.inquire(std::get<0>(qry[i])) == std::get<1>(qry[i]);
    }
    for (int i = 1; i <= q; i++)
        puts(ans[i] ? "YES" : "NO");
}