#include <cstdio>
#include <set>
constexpr int N = 500005;
std::set<int> st[N];
int a[N], n, w;
inline int nearL(int p, int x) {
    auto it = st[x].lower_bound(p);
    if (it == st[x].begin()) return 0;
    else return *(--it);
}
inline int nearR(int p, int x) {
    auto it = st[x].upper_bound(p);
    if (it == st[x].end()) return n + 1;
    else return *it;
}
inline int inquireR(int p) {
    int posP = nearR(p, a[p]), posQ = nearR(p, w-a[p]);
    return posP < posQ ? n+1 : posQ;
}
struct SegTr {
    int tr[N << 2];
    inline void pushup(int u) { tr[u] = std::min(tr[u << 1], tr[u << 1 | 1]); }
    void build(int u=1, int l=1, int r=n) {
        if (l == r) tr[u] = inquireR(l);
        else {
            int mid = l + r >> 1;
            build(u << 1, l, mid);
            build(u << 1 | 1, mid + 1, r);
            pushup(u);
        }
    }
    int p, x, ql, qr;
    void upd(int u, int l, int r) {
        if (l == r) tr[u] = x;
        else {
            int mid = l + r >> 1;
            if (p <= mid) upd(u << 1, l, mid);
            else upd(u << 1 | 1, mid + 1, r);
            pushup(u);
        }
    }
    int que(int u, int l, int r) {
        if (ql <= l && r <= qr) return tr[u];
        int mid = l + r >> 1, res = n + 1;
        if (ql <= mid) res = std::min(res, que(u << 1, l, mid));
        if (mid < qr) res = std::min(res, que(u << 1 | 1, mid + 1, r));
        return res;
    }
    inline void update(int pos, int val) { p = pos, x = val; upd(1, 1, n); }
    inline int inquire(int l, int r) { ql = l, qr = r; return que(1, 1, n); }
} fmn;
/*
易于发现只有最近的互补点对（和为 w）才有贡献。改掉一个数之后，最近互补点对变化是 O(1) 的。直接维护即可。
*/
int main() {
    int m, cnt = 0;
    scanf("%d%d%d", &n, &m, &w);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    for (int i = 1; i <= n; i++)
        st[a[i]].insert(i);
    fmn.build();
    for (int opt, x, y; m--; ) {
        scanf("%d%d%d", &opt, &x, &y);
        if (opt == 1) {
            int befQL = nearL(x, w-a[x]), befQR = nearR(x, w-a[x]);
            st[a[x]].erase(x); st[a[x] = y].insert(x);
            int aftQL = nearL(x, w-y), aftQR = nearR(x, w-y);
            // printf("[%d  %d  %d]\n", befQL, x, befQR);
            // printf("[%d  %d  %d]\n", aftQL, x, aftQR);
            if (befQL) fmn.update(befQL, inquireR(befQL));
            if (int befL = nearL(befQR, w-a[befQR])) fmn.update(befL, inquireR(befL));
            if (aftQL) fmn.update(aftQL, inquireR(aftQL));
            if (int aftL = nearL(aftQR, w-a[aftQR])) fmn.update(aftL, inquireR(aftL));
            fmn.update(x, inquireR(x));
            // for (int i = 1; i <= n; i++)
            //     printf("%d%c", fmn.inquire(i,i), " \n"[i==n]);
        } else {
            x ^= cnt, y ^= cnt;
            bool ans = fmn.inquire(x, y) <= y;
            // printf("%d\n", fmn.inquire(x, y));
            cnt += ans;
            puts(ans ? "Yes" : "No");
        }
    }
    return 0;
}
