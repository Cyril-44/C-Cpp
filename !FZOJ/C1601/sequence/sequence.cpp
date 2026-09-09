#include <cstdio>
#include <cstdint>
#include <queue>
#include <algorithm>
#include <cassert>
struct FastI {
    char buf[1 << 20], *p1{}, *p2{};
    [[gnu::always_inline]] char gc() {
        if (p1 == p2) p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
        if (p1 == p2) return EOF;
        return *p1++;
    }
    template<class T> void operator()(T &x) {
        char ch = gc();
        while (ch < '0' || ch > '9') ch = gc();
        for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
} in;
struct FastO {
    char buf[1 << 20], *p1{buf};
    const char *p2{buf + sizeof(buf)};
    [[gnu::always_inline]] void pc(char ch) {
        if (p1 == p2) fwrite(buf, sizeof buf, 1, stdout), p1=buf;
        *p1++ = ch;
    }
    void flush() { fwrite(buf, p1-buf, 1, stdout), p1=buf; }
    template<class T> void operator()(T x) {
        static uint8_t sta[64];
        sta[sta[0] = 1] = x % 10 | '0';
        while (x /= 10) sta[++sta[0]] = x % 10 | '0';
        while (sta[0]) pc(sta[sta[0]--]);
    }
    ~FastO() { flush(); }
} out;
#ifdef CLANGD
constexpr int N = 14;
#else
constexpr int N = 1000004;
#endif
constexpr int INF = 0x7fffffff;
int a[N], tm[N], n;
struct Max {
    int mx = -INF, id, tm;
    Max& operator+=(Max o) {
        if (o.mx > mx) *this = o;
        return *this;
    }
    friend Max operator+(Max x, Max y) { return x += y; }
};
class SegTr {
    Max tr[N << 2];
    int P, L, R;
    void upd(int u, int l, int r) {
        if (l == r) { tr[u] = {a[l], l, tm[l]}; return; }
        int mid = l + r >> 1;
        P <= mid ? upd(u<<1, l, mid) : upd(u<<1|1, mid+1, r);
        tr[u] = tr[u<<1] + tr[u<<1|1];
    }
    Max inq(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u];
        int mid = l + r >> 1;
        if (R <= mid) return inq(u<<1, l, mid);
        if (mid < L) return inq(u<<1|1, mid+1, r);
        return inq(u<<1, l, mid) + inq(u<<1|1, mid+1, r);
    }
public:
    void modify(int p) { P=p; upd(1, 1, n); }
    Max inquire(int l, int r) { L=l, R=r; return inq(1, 1, n); }
    void build(int u=1, int l=1, int r=n) {
        if (l == r) { tr[u] = {a[l], l, 0}; return; }
        int mid = l + r >> 1;
        build(u<<1, l, mid), build(u<<1|1, mid+1, r);
        tr[u] = tr[u<<1] + tr[u<<1|1];
    }
} f;
struct Candidate {
    int ans, xid, xtm, yid, ytm;
    bool operator<(const Candidate& o) const {
        return ans < o.ans;
    }
};
int main() {
    int k, q, online;
    in(n), in(k), in(q), in(online);
    for (int i = 1; i <= n; i++) in(a[i]);
    f.build();
    std::priority_queue<Candidate> candidates;
    auto chk = [&](int x) {
        if (x > 1) {
            auto res = f.inquire(std::max(1, x-k), x-1);
            candidates.push({a[x] + res.mx, x, tm[x], res.id, res.tm});
        }
        if (x < n) {
            auto res = f.inquire(x+1, std::min(n, x+k));
            candidates.push({a[x] + res.mx, x, tm[x], res.id, res.tm});
        }
    };
    for (int i = 1; i <= n; i++) chk(i);
    int lastans = 0;
    out(lastans = candidates.top().ans), out.pc('\n');
    for (int x, y; q--; ) {
        in(x), in(y);
        if (online) x ^= lastans, y ^= lastans;
        ++tm[x]; a[x] = y; f.modify(x);
        if (x > 1) {
            auto res = f.inquire(std::max(1, x-k), x-1);
            candidates.push({y + res.mx, x, tm[x], res.id, res.tm});
            chk(res.id);
        }
        if (x < n) {
            auto res = f.inquire(x+1, std::min(n, x+k));
            candidates.push({y + res.mx, x, tm[x], res.id, res.tm});
            chk(res.id);
        }
        auto ans = candidates.top();
        while (ans.xtm < tm[ans.xid] || ans.ytm < tm[ans.yid])
            candidates.pop(), assert(!candidates.empty()), ans = candidates.top();
        out(lastans = ans.ans), out.pc('\n');
    }
    return 0;
}