#include <cstdio>
#include <cstdint>
#include <algorithm>
#ifdef CLANGD
constexpr int N = 44;
#else
constexpr int N = 400004;
#endif
int p[N], pos[N];
struct Data {
    int min, minpos;
    friend Data operator+(const Data& l, const Data& r) {
        Data res;
        if (l.min <= r.min)
            res.min = l.min, res.minpos = l.minpos;
        else
            res.min = r.min, res.minpos = r.minpos;
        return res;
    }
    int operator()() const { return min; }
};
int n;
struct SegTr {
    struct Node {
        Data dat;
        int add;
        void pull(int a) { dat.min += a, add += a; }
    } tr[N << 2];
    int L, R, X;
    void pushdown(int u) {
        if (tr[u].add) {
            tr[u<<1].pull(tr[u].add);
            tr[u<<1|1].pull(tr[u].add);
            tr[u].add = 0;
        }
    }
    void pushup(int u) { tr[u].dat = tr[u<<1].dat + tr[u<<1|1].dat; }
    void upd(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].pull(X);
        int mid = l + r >> 1; pushdown(u);
        if (L <= mid) upd(u<<1, l, mid);
        if (mid < R) upd(u<<1|1, mid+1, r);
        pushup(u);
    }
    Data inq(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].dat;
        int mid = l + r >> 1; pushdown(u);
        if (R <= mid) return inq(u<<1, l, mid);
        if (mid < L) return inq(u<<1|1, mid+1, r);
        return inq(u<<1, l, mid) + inq(u<<1|1, mid+1, r);
    }
    int presum = 0;
    void build(int u, int l, int r) {
        if (l == r) return void(tr[u] = {{presum += (p[l] < n+1 ? -1 : p[l] == n+1 ? 0 : 1), l}, 0});
        int mid = l + r >> 1;
        build(u<<1, l, mid), build(u<<1|1, mid+1, r);
        pushup(u);
    }
public:
    int all;
    void rangeAdd(int l, int r, int x) { L=l, R=r, X=x; upd(1, 1, all); }
    void diffAdd(int pos, int x) { rangeAdd(pos, all, x); }
    Data inquire(int l, int r) { L=l, R=r; return inq(1, 1, all); }
    int findMinPos() const { return tr[1].dat.minpos; }
    int getMin(int pos) { return inquire(pos, pos + 2*n).min; }
    void build() { all = 2*(2*n+1); build(1, 1, all); }
} f;
inline int dis(int u, int v) {
    if (pos[u] > pos[v]) return pos[v] + 2*n+1 - pos[u];
    return pos[v] - pos[u];
}
int main() {
    int q;
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= 2*n+1; i++)
        scanf("%d", &p[i]), p[i + 2*n+1] = p[i], pos[p[i]] = i;
    f.build();
    int64_t loRing = dis(n+1, 1), hiRing = dis(2*n+1, n+1);
    for (int i = 1; i <= n; i++) loRing += dis(i, i+1);
    for (int i = n+1; i <= 2*n; i++) hiRing += dis(i, i+1); 
    auto to0n = [](int x) { return x == 2*n+1 ? 0 : x; };
    auto fix = [&](int v, int flg) {
        if (v == 1) loRing += flg * (dis(1, 2) + dis(n+1, 1));
        else if (v == n+1) loRing += flg * (dis(n, n+1) + dis(n+1, 1)), hiRing += flg * (dis(n+1, n+2) + dis(2*n+1, n+1));
        else if (v == 2*n+1) hiRing += flg * (dis(2*n, 2*n+1) + dis(2*n+1, n+1));
        else if (v < n+1) loRing += flg * (dis(v-1, v) + dis(v, v+1));
        else hiRing += flg * (dis(v-1, v) + dis(v, v+1));
        if (v < n+1) f.diffAdd(pos[v], -flg), f.diffAdd(pos[v] + 2*n+1, -flg);
        else if (v > n+1) f.diffAdd(pos[v], flg), f.diffAdd(pos[v] + 2*n+1, flg);
    };
    while (q--) {
        int u, v;
        scanf("%d%d", &u, &v);
        fix(p[u], -1), fix(p[v], -1);
        std::swap(pos[p[u]], pos[p[v]]);
        std::swap(p[u], p[v]), std::swap(p[u+2*n+1], p[v+2*n+1]);
        fix(p[u], 1), fix(p[v], 1);
        if (f.inquire(pos[n+1], pos[n+1])() != f.getMin(pos[n+1])) {
            // fprintf(stderr, "Circumstance 1: ");
            printf("%d\n", to0n(f.findMinPos()));
        } else if (loRing != 2*n+1) {
            // fprintf(stderr, "Circumstance 2: ");
            printf("%d\n", to0n(pos[n+1]));
        } else if (hiRing != 2*n+1) {
            // fprintf(stderr, "Circumstance 3: ");
            printf("%d\n", pos[n+1] - 1);
        } else puts("-1");
    }
    return 0;
}