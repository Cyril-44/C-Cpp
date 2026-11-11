#include <bits/stdc++.h>
constexpr int N = 100000;
int n;
static void apply1(uint64_t &x, uint64_t y) { x |= y; }
static void apply0(uint64_t &x, uint64_t y) { x &= ~0ull ^ y; }
static void applyf(uint64_t &x, uint64_t y) { x ^= y; }
struct Count {
    static int cnt;
    static void applyc(uint64_t &x, uint64_t y) {
        cnt += __builtin_popcountll(x & y);
    }
};
int Count::cnt;
struct Bitset {
    constexpr static int B = 64, MX = N / B + 5;
    uint64_t a[MX]{};
    void in() {
        char ch = getchar();
        while (ch < '0') ch = getchar();
        for (int i = 0; i < n; i++, ch = getchar())
            if (ch ^ '0') a[i / B] |= 1ull << i % B;
    }
    template<void (apply)(uint64_t&, uint64_t)>
    void set(int l, int r) {
        if (l/B == r/B) apply(a[l/B], ~0ull << B-1-r%B >> B-1-r%B+l%B << l%B);
        else {
            apply(a[l/B], ~0ull >> l%B << l%B);
            apply(a[r/B], ~0ull >> B-1-r%B);
            for (int i = l/B + 1; i < r/B; ++i)
                apply(a[i], ~0ull);
        }
    }
    template<auto apply>
    Bitset get(int l, int r) { Bitset res = *this; res.set<apply>(l, r); return res; }
    Bitset& operator&=(const Bitset &o) { for (int i = 0; i < MX; i++) a[i] &= o.a[i]; return *this; }
    Bitset& operator|=(const Bitset &o) { for (int i = 0; i < MX; i++) a[i] |= o.a[i]; return *this; }
    Bitset& operator^=(const Bitset &o) { for (int i = 0; i < MX; i++) a[i] ^= o.a[i]; return *this; }
    Bitset& operator>>=(int n) {
        int jmp = n / B, lo = n % B;
        for (int i = 0; i + jmp + 1 < MX; i++) {
            if (lo) a[i] = (a[i + jmp] & ~0ull >> lo << lo) >> lo | (a[i + jmp + 1] & ~0ull >> B-lo) << B-lo;
            else a[i] = a[i + jmp];
        }
        for (int i = MX - jmp - 1; i < MX; i++) a[i] = 0;
        return *this;
    }
    friend Bitset operator&(Bitset x, const Bitset &y) { return x &= y; }
    friend Bitset operator|(Bitset x, const Bitset &y) { return x |= y; }
    friend Bitset operator^(Bitset x, const Bitset &y) { return x ^= y; }
    friend Bitset operator>>(Bitset x, int y) { return x >>= y; }
    Bitset operator~() const {
        Bitset res = *this;
        for (int i = 0; i < MX; i++) res.a[i] = ~res.a[i];
        return res;
    }
    int popcount(int l, int r) { Count::cnt = 0; set<Count::applyc>(l, r); return Count::cnt; }
    bool operator[](int p) const { return a[p/B] >> p%B & 1; }
} s;
struct Node {
    Bitset q[3];
    int sz;
    void push(const Bitset &o) { q[sz++] = o; }
    Bitset pop3() {
        auto res = (q[0] & q[1]) | (q[0] & q[2]) | (q[1] & q[2]);
        q[0] ^= q[1] ^ q[2], sz = 1;
        return res;
    }
    Bitset pop2() {
        auto res = q[0] & q[1];
        q[0] ^= q[1], sz = 1;
        return res;
    }
    Bitset operator()() const { return q[0]; }
} bit[20];
int main() {
    freopen("parity.in", "r", stdin);
    freopen("parity.out", "w", stdout);
    int m;
    scanf("%d%d", &n, &m);
    s.in();
    for (int op, l, r, lk, d, h; m--; ) {
        scanf("%d%d%d", &op, &l, &r);
        --l, --r;
        if (op == 1) {
            scanf("%d", &d);
            if (d) s.set<apply1>(l, r);
            else s.set<apply0>(l, r);
        } else if (op == 2) {
            s.set<applyf>(l, r);
        } else {
            scanf("%d%d%d", &lk, &d, &h);
            int top = 0;
            auto add = [&](const Bitset &t) {
                bit[0].push(t);
                for (int k = 0; k <= top; k++)
                    if (bit[k].sz == 3) {
                        bit[k+1].push(bit[k].pop3());
                        if (k == top) ++top;
                    }
            };
            for (int p = 1; p <= lk; p++) {
                int a, b, c, v;
                scanf("%d%d%d%d", &a, &b, &c, &v);
                auto inq = s>>a-1 ^ s>>b-1 ^ s>>c-1;
                if (!v) inq.set<applyf>(0, n-1);
                add(inq);
            }
            for (int k = 0; k <= top; k++)
                if (bit[k].sz == 3) {
                    bit[k+1].push(bit[k].pop3());
                    if (k == top) ++top;
                } else if (bit[k].sz == 2) {
                    bit[k+1].push(bit[k].pop2());
                    if (k == top) ++top;
                }
            Bitset used;
            int res = 0, cnt = 0;
            h = (r-l-d+2) + 1 - h;
            for (int k = top; k >= 0; k--) {
                auto curr = bit[k]().get<applyf>(0, n-1) & ~used;
                int currcnt = curr.popcount(l, r - d + 1);
                if (cnt + currcnt < h) cnt += currcnt, used |= curr, res |= 1<<k;
                else used |= bit[k]();
                bit[k].sz = 0;
            }
            printf("%d\n", res);
        }
        
    }
}
