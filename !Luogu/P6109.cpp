#include <bits/stdc++.h>
namespace FastI {
    char buf[1 << 20], *p1, *p2;
    inline char gc() {
        if (p1 == p2) {
            p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
            if (p1 == p2) return -1;
        }
        return *p1++;
    }
    inline void in(int &x) {
        char ch = gc();
        while (ch < 0) ch = gc();
        for (x = 0; ch >= '0'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
} using FastI::in;
struct FastO {
    char buf[1 << 20], *p1 = buf;
    const char *p2 = buf + sizeof(buf);
    void pc(char ch) {
        if (p1 == p2) fwrite(buf, sizeof buf, 1, stdout), p1=buf;
        *p1++ = ch;
    }
    void flush() { fwrite(buf, p1-buf, 1, stdout), p1=buf; }
    ~FastO() { flush(); }
    void operator()(int64_t x, char follow = '\n') {
        if (x < 0) pc('-'), x = -x;
        int8_t sta[20];
        sta[sta[0] = 1] = x % 10;
        while (x /= 10) sta[++sta[0]] = x % 10;
        while (sta[0]) pc(sta[sta[0]--] | '0');
        if (follow) pc(follow);
    }
} out;
constexpr int N = 50005, M = 500005;
#ifdef CLANGD
#define N 5
#define M 55
#endif
std::vector<std::tuple<int,int,int>> mods[N];
std::vector<std::tuple<int,int,int,int>> lqrys[N<<2], rqrys[N<<2];
int64_t ans[M];
int n;
struct Insert {
    int L, R, xl, xr, id;
    void work(int u=1, int l=1, int r=n) {
        int mid = l + r >> 1;
        if (l == r) lqrys[u].emplace_back(L, xl, xr, id);
        else if (R <= mid) work(u<<1, l, mid);
        else if (mid < L) work(u<<1|1, mid+1, r);
        else {
            lqrys[u].emplace_back(L, xl, xr, id);
            rqrys[u].emplace_back(R, xl, xr, id);
        }
    }
    Insert(int i) : id(i) { in(L), in(xl), in(R), in(xr); }
};
std::vector<std::tuple<int,int,int>> nodes[20];
void fetch(int u=1, int l=1, int r=n) {
    nodes[31 - __builtin_clz(u)].emplace_back(u, l, r);
    if (l == r) return;
    int mid = l + r >> 1;
    fetch(u<<1, l, mid), fetch(u<<1|1, mid+1, r);
}
struct SegTr {
    std::vector<int> backup;
    struct Node {
        int64_t mx, add, hmx, hadd;
        bool st;
        void pull(int64_t ad, int64_t had = 0) {
            hadd = std::max(add += ad, had);
            hmx = std::max(mx += ad, hmx + had);
        }
        void pull2(int64_t ad) { hmx = mx += ad, add += ad; }
    } tr[N << 2], bak[N << 2];
    int L, R, X;
    void check(int u) {
        if (!tr[u].st) {
            backup.emplace_back(u);
            bak[u] = tr[u];
            tr[u].st = true;
        }
    }
    void pushdown(int u) {
        check(u<<1), check(u<<1|1);
        tr[u<<1].pull(tr[u].add, tr[u].hadd);
        tr[u<<1|1].pull(tr[u].add, tr[u].hadd);
        tr[u].add = tr[u].hadd = 0;
    }
    void pushdown2(int u) {
        tr[u<<1].pull2(tr[u].add);
        tr[u<<1|1].pull2(tr[u].add);
        tr[u].add = 0;
    }
    void pushup(int u) {
        tr[u].mx = std::max(tr[u<<1].mx, tr[u<<1|1].mx);
        tr[u].hmx = std::max(tr[u<<1].hmx, tr[u<<1|1].hmx);
    }
    void upd(int u, int l, int r) {
        check(u);
        if (L <= l && r <= R) return tr[u].pull(X);
        int mid = l + r >> 1;
        pushdown(u);
        if (L <= mid) upd(u<<1, l, mid);
        if (mid < R) upd(u<<1|1, mid+1, r);
        pushup(u);
    }
    void upd2(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].pull2(X);
        int mid = l + r >> 1;
        pushdown2(u);
        if (L <= mid) upd2(u<<1, l, mid);
        if (mid < R) upd2(u<<1|1, mid+1, r);
        pushup(u);
    }
    int64_t inq(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].hmx;
        int mid = l + r >> 1;
        check(u); pushdown(u);
        if (R <= mid) return inq(u<<1, l, mid);
        if (mid < L) return inq(u<<1|1, mid+1, r);
        return std::max(inq(u<<1, l, mid), inq(u<<1|1, mid+1, r));
    }
    void update(int l, int r, int x) { L=l, R=r, X=x; upd(1, 1, n); }
    void update2(int l, int r, int x) { L=l, R=r, X=x; upd2(1, 1, n); }
    int64_t inquire(int l, int r) { L=l, R=r; return inq(1, 1, n); }
    void restore() {
        for (int u : backup) tr[u] = bak[u];
        backup.clear();
    }
} f;
int main() {
    int m, q;
    in(n), in(m), in(q);
    for (int l1, l2, r1, r2, x; m--; ) {
        in(l1), in(l2), in(r1), in(r2), in(x);
        mods[l1].emplace_back(l2, r2, x);
        mods[r1+1].emplace_back(l2, r2, -x);
    }
    for (int i = 1; i <= q; i++)
        Insert(i).work();
    fetch();
    for (int dep = 0; !nodes[dep].empty(); dep++) {
        memset(f.tr, 0, sizeof f.tr);
        // fprintf(stderr, "===== Dep %d ====\n", dep);
        for (const auto &[u, l, r] : nodes[dep]) {
            // fprintf(stderr, "======== u=%d, [%d,%d] =======\n", u, l, r);
            int mid = l + r >> 1, idx = mid;
            for (int i = l; i <= mid; i++)
                for (const auto &[ml, mr, x] : mods[i]) {
                    f.update2(ml, mr, x);
                    // fprintf(stderr, "f2 [%d,%d] += %d\n", ml, mr, x);
                }

            // fprintf(stderr, "=== Lqry ===\n");
            std::sort(lqrys[u].rbegin(), lqrys[u].rend());
            for (const auto &[L, ql, qr, id] : lqrys[u]) {
                for (; idx > L; idx--)
                    for (const auto &[ml, mr, x] : mods[idx]) {
                        f.update(ml, mr, -x);
                        // fprintf(stderr, "idx=%d, f [%d,%d] += %d\n", idx, ml, mr, x);
                    }
                // fprintf(stderr, "ans[%d] <-- %lld\n", id, f.inquire(ql, qr));
                ans[id] = std::max(ans[id], f.inquire(ql, qr));
            }
            f.restore();

            // fprintf(stderr, "=== Rqry ===\n");
            std::sort(rqrys[u].begin(), rqrys[u].end());
            idx = mid+1;
            for (const auto &[R, ql, qr, id] : rqrys[u]) {
                for (; idx <= R; idx++)
                    for (const auto &[ml, mr, x] : mods[idx]) {
                        f.update(ml, mr, x);
                        // fprintf(stderr, "idx=%d, f [%d,%d] += %d\n", idx, ml, mr, x);
                    }
                // fprintf(stderr, "R=%d, [%d,%d] ans[%d] <-- %lld\n", R, ql, qr, id, f.inquire(ql, qr));
                ans[id] = std::max(ans[id], f.inquire(ql, qr));
            }
            f.restore();

            for (int i = mid+1; i <= r; i++)
                for (const auto &[ml, mr, x] : mods[i]) {
                    f.update2(ml, mr, x);
                    // fprintf(stderr, "f2 [%d,%d] += %d\n", ml, mr, x);
                }
        }
    }
    for (int i = 1; i <= q; i++)
        out(ans[i]);
}
