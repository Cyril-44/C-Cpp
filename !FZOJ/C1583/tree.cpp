#include <cstdio>
#include <algorithm>
#include <cstring>
#define For(i, s, t) for(int i = (s); i <= (t); i++)
constexpr int N = 500005;
struct FastI {
    char get() {
        if (p1 == p2) p2 = (*buf='\0', p1=buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++;
    }
    void operator()(int &x) {
        char ch = get();
        while (ch < '0' || ch > '9') ch = get();
        for (x = 0; ch >= '0' && ch <= '9'; ch = get())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
private:
    char buf[1 << 20], *p1{}, *p2{};
} fin; 
int n;
struct FastO {
    FastO() : p3(buf), p4(buf + sizeof(buf)) {}
    ~FastO() { flush(); }
    void flush() { fwrite(buf, p3-buf, 1, stdout); }
    void put(char ch) {
        if (p3 == p4) fwrite(buf, sizeof buf, 1, stdout), p3 = buf;
        *p3++ = ch;
    }
    void operator()(int x) {
        if (x == -1) return put('-'), put('1');
        static unsigned char buf[20];
        buf[buf[0] = 1] = x % 10;
        while (x /= 10) buf[++buf[0]] = x % 10;
        while (buf[0]) put(buf[buf[0]--] | '0');
    }
private:
    char buf[1 << 20], *p3;
    const char *p4;
} fout;

struct Node {
    int sum, cov;
} tr[N << 2];
std::pair<int,int> op1[N], op0[N];
#define ls (u<<1)
#define rs (u<<1|1)
struct SegTr {
    void pushup(int u) {
        tr[u].sum = tr[ls].sum + tr[rs].sum;
    }
    void pushdown(int u, int lc, int rc) {
        if (~tr[u].cov) {
            tr[ls].sum = tr[u].cov * lc;
            tr[rs].sum = tr[u].cov * rc;
            tr[ls].cov = tr[rs].cov = tr[u].cov;
            tr[u].cov = -1;
        }
    }
    void build(int u=1, int l=1, int r=n) {
        tr[u].cov = -1;
        if (l == r) tr[u].sum = 1;
        else {
            int mid = l + r >> 1;
            build(ls, l, mid);
            build(rs, mid+1, r);
            pushup(u);
        }
    }
    int X, L, R;
    int inq(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].sum;
        int mid = l + r >> 1, res = 0;
        pushdown(u, mid-l+1, r-mid);
        if (L <= mid) res += inq(ls, l, mid);
        if (mid < R) res += inq(rs, mid+1, r);
        return res;
    }
    void cov(int u, int l, int r) {
        if (L <= l && r <= R) tr[u] = {X * (r-l+1), X};
        else {
            int mid = l + r >> 1;
            pushdown(u, mid-l+1, r-mid);
            if (L <= mid) cov(ls, l, mid);
            if (mid < R) cov(rs, mid+1, r);
            pushup(u);
        }
    }
    int inquire(int l, int r) {
        L=l, R=r; return inq(1, 1, n);
    }
    void cover(int l, int r, bool op) {
        X = op, L=l, R=r; cov(1, 1, n);
    }
} sgt;

int main() {
    // freopen("tree1.in", "r", stdin);
    int q, a, o;
    fin(n), fin(q), fin(a), fin(o);
    For(i, 1, q) {
        int l, r; fin(l), fin(r);
        op0[i] = {l, r};
        if (i+a <= q) op1[i+a] = {l, r};
    }
    sgt.build();
    For(i, 1, q) {
        if (op1[i].second) sgt.cover(op1[i].first, op1[i].second, 1);
        fout(op0[i].second ? sgt.inquire(op0[i].first, op0[i].second) : 0), fout.put(' ');
        if (op0[i].second) sgt.cover(op0[i].first, op0[i].second, 0);
        fout(sgt.inquire(1, n)), fout.put('\n');
    }
    return 0;
}