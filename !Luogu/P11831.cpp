#include <cstring>
#include <cstdio>
#include <vector>
constexpr int N = 100005, B = 320, S = 64;
#define UFor(i, s, t) for (unsigned i = s; i ^ t; ++i)
#define For(i, s, t) for (int i = s; i <= t; ++i)
#define roF(i, t, s) for (int i = t; i >= s; --i)
struct Bitset {
    constexpr static int N = (::N+S-1) / S + 1;
    unsigned long a[N]{};
    inline void reset() { memset(a, 0, sizeof a); }
    inline void set() { memset(a, 0xff, sizeof a); }
    inline void set(unsigned p) { a[p >> 6] |= 1ul << (p & 63); }
    inline void reset(unsigned p) { a[p >> 6] &= ~(1ul << (p & 63)); }
    inline void flip(unsigned p) { a[p >> 6] ^= 1ul << (p & 63); }
    inline bool operator[](unsigned p) const { return a[p >> 6] >> (p & 63) & 1; }
    inline unsigned long& operator()(unsigned p) { return a[p]; }
    inline Bitset& operator&=(const Bitset& o) { UFor(i, 0, N) a[i] &= o.a[i]; return *this; }
    inline Bitset& operator^=(const Bitset& o) { UFor(i, 0, N) a[i] ^= o.a[i]; return *this; }
    inline Bitset& operator|=(const Bitset& o) { UFor(i, 0, N) a[i] |= o.a[i]; return *this; }
    inline Bitset operator&(Bitset o) const { return o &= *this; }
    inline Bitset operator^(Bitset o) const { return o ^= *this; }
    inline Bitset operator|(Bitset o) const { return o |= *this; }
    inline bool operator==(const Bitset &o) const { return !memcmp(a, o.a, sizeof a); }
    inline bool operator!=(const Bitset &o) const { return  memcmp(a, o.a, sizeof a); }
    inline void output(const int s = 0, const int t = N * S, FILE* f = stdout) {
        UFor(j, s, t) fputc((*this)[j] | '0', f);
    }
} age[B], bge[B], aval[N];
/* 当前的 S = 64
age[i]：维护 a_p \ge S*i+1 的 p 集合
bge[i]：维护 b_p \ge S*i+1 的 p 集合
aval[i]：维护 i 可达的所有点集合。
*/
int a[N], pa[N], b[N], pb[N];
std::vector<int> g[N];
inline void solveSingle() {
    int n, m, q;
    scanf("%d%d%d", &n, &m, &q);
    For(i, 1, n) g[i].clear();
    for (int u, v; m--; ) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
    }
    roF(u, n, 1) { aval[u].reset(); aval[u].set(u); for (int v : g[u]) aval[u] |= aval[v]; }
    For(i, 1, n) scanf("%d", &a[i]), pa[a[i]] = i;
    For(i, 1, n) scanf("%d", &b[i]), pb[b[i]] = i;
    int lastblock = -1;
    const int lastBitsetBlock = n >> 6;
    UFor(i, 0, B) {
        age[i].reset(), bge[i].reset();
        if ((i+1) * B >= 1u*n) {
            For(j, i*B+1, n) age[i].set(pa[j]), bge[i].set(pb[j]);
            lastblock = i; break;
        }
        For(j, i*B+1, (i+1)*B) age[i].set(pa[j]), bge[i].set(pb[j]);
    }
    roF(i, lastblock - 1, 0) age[i] |= age[i+1], bge[i] |= bge[i+1];
#define bswap(var, x, y) do {                                       \
    int bl = (var[x]-1) / B, br = (var[y]-1) / B;                   \
    if (bl > br) std::swap(bl, br);                                 \
    For(i, bl+1, br) var##ge[i].flip(x), var##ge[i].flip(y);        \
    std::swap(p##var[var[x]], p##var[var[y]]);                      \
    std::swap(var[x], var[y]);                                      \
} while (0)
    for (int o, x, y, z, qid=1; qid<=q; ++qid) {
        scanf("%d%d%d", &o, &x, &y);
        switch (o) {
        case 1: bswap(a, x, y); break;
        case 2: bswap(b, x, y); break;
        case 3: { // query max_{x->i and y<=a_i<=z} b_is
            scanf("%d", &z);
            int blkL = (y-1) / B, blkR = (z-1) / B;
            Bitset sat = age[blkL] ^ age[blkR];
            For(p, blkR * B + 1, z) sat.set(pa[p]);
            UFor(p, blkL * B + 1, y) sat.reset(pa[p]);
            sat &= aval[x];
            int blk = -1;
            For(j, 0, lastBitsetBlock)
                while (sat(j) & bge[blk+1](j)) { if (++blk == lastblock) break; }
            if (blk == -1) puts("0");
            else roF(i, std::min(n, B * (blk+1)), B * blk + 1)
                if (sat[pb[i]]) {
                    printf("%d\n", i);
                    break;
                }
        } break;
        }
    }
}
int main() {
#ifndef ONLINE_JUDGE
    freopen("recall.in", "r", stdin);
    freopen("recall.out", "w", stdout);
#endif
    int c, T; scanf("%d%d", &c, &T); while (T--) solveSingle();
    return 0;
}
