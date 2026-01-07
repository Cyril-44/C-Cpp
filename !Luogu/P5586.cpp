#include <cfloat>
#include <cstdio>
#include <cstdint>
#include <cassert>
#include <array>
#include <vector>
#include <algorithm>
#include <type_traits>
constexpr int N = 300005, MOD = (unsigned)1e9 + 7;
int a[N];
enum ProtoType { COV, ADD, FLP, COPY, SWAP };
#define Inline __attribute__((always_inline)) inline
class FastIS{static constexpr size_t h=1<<20;char o[h];char*t,*v;FILE*K;public:FastIS(FILE*B=stdin):t(nullptr),v(nullptr),K(B){}~FastIS(){fclose(K);}Inline int get(){return(t==v)&&(v=(t=o)+fread(o,1,h,K),t==v)?EOF:*t++;}template<typename j>typename std::enable_if<std::is_unsigned<j>::value&&std::is_integral<j>::value,FastIS&>::type inline operator>>(j&d){int a=get();while(~a&&(a<'0'||a>'9'))a=get();for(d=0;a>='0'&&a<='9';a=get())d=(d<<3)+(d<<1)+(a^'0');return*this;}template<typename j>typename std::enable_if<std::is_signed<j>::value&&std::is_integral<j>::value&&!std::is_same<j,bool>::value&&!std::is_same<j,char>::value,FastIS&>::type inline operator>>(j&d){int a=get();bool L=false;while(~a&&(a<'0'||a>'9')&&(a^'-'))a=get();if(a=='-')a=get(),L=true;for(d=0;a>='0'&&a<='9';a=get())d=(d<<3)+(d<<1)+(a^'0');d=L?-d:d;return*this;}template<typename j>typename std::enable_if<std::is_floating_point<j>::value,FastIS&>::type inline operator>>(j&d){int a=get();bool L=false;while(~a&&(a<'0'||a>'9')&&(a^'-'))a=get();if(a=='-')a=get(),L=true;using G=typename std::conditional<std::is_same<j,float>::value,std::uint32_t,std::uint64_t>::type;G I;for(I=0;a>='0'&&a<='9';a=get())I=(I<<3)+(I<<1)+(a^'0');d=I;if(a=='.'){G w=1;for(a=get();a>='0'&&a<='9';a=get())d+=(a^'0')*(1./(w*=10));}if(L)d=-d;return*this;}inline FastIS&operator>>(bool&U){int a=get();while(~a&&a!='0'&&a!='1')a=get();U=static_cast<bool>(a&1);return*this;}inline FastIS&operator>>(char&n){int a=get();while(a==' '||a=='\r'||a=='\n')a=get();n=a;return*this;}inline FastIS&operator>>(char*n){int a=get();while(~a&&(a==' '||a=='\r'||a=='\n'))a=get();while(~a&&(a!=' '&&a!='\r'&&a!='\n'))*n++=a,a=get();return*this;}}fin;class FastOS{static constexpr size_t h=1<<20;static constexpr double R=1e-9;char o[h],*O;const char*y;FILE*q;int l;public:FastOS(FILE*B=stdout):O(o),y(o+h),q(B),l(-1){setvbuf(q,nullptr,_IONBF,0);}~FastOS(){flush();fclose(q);}FastOS&flush(){fwrite(o,O-o,1,q);return*this;}FastOS&put(int H){*O++=H;if(O==y)fwrite(o,h,1,q),O=o;return*this;}FastOS&setprecision(const int&S){l=S;return*this;}template<typename j>typename std::enable_if<std::is_unsigned<j>::value&&std::is_integral<j>::value,FastOS&>::type inline operator<<(j d){static int n[32];n[*n=1]=d%10;while(d/=10)n[++(*n)]=d%10;while(*n)put(n[(*n)--]|'0');return*this;}template<typename j>typename std::enable_if<std::is_signed<j>::value&&std::is_integral<j>::value&&!std::is_same<j,char>::value,FastOS&>::type inline operator<<(const j&d){if(d<0)put('-')<<(typename std::make_unsigned<j>::type)(-d);else*this<<(typename std::make_unsigned<j>::type)(d);return *this;}template<typename j>typename std::enable_if<std::is_floating_point<j>::value,FastOS&>::type inline operator<<(j d){if(d<0)put('-'),d=-d;using G=typename std::conditional<std::is_same<j,float>::value,std::uint32_t,std::uint64_t>::type;G z=static_cast<G>(d);j O=d-z;*this<<z;if(l>0||l<0&&O>R){put('.');for(int J=0;l>0&&J<l||l<0&&O>R;J++){int N=static_cast<int>(O*=10);put(N|'0');O-=N;}}return*this;}FastOS&operator<<(const char&H){put(H);return*this;}}fout;
namespace WBLT {
#define U tr[u]
inline void plus(int &x, int y) { if ((x += y) >= MOD) x -= MOD; }
enum SonType {L, R};
struct Node {
    int ch[2];
    int sz, refCnt;
    int sum;
    Node& operator()(bool);
    inline int& operator[](bool c) { return ch[c]; }
    explicit Node (int v = 0) : ch{}, sz(1), refCnt(1), sum(v) {}
    explicit Node (int ls, int rs) : ch{ls, rs}, sz(), refCnt(1), sum() {}
    struct Tag {
        int cov, add; bool rev;
        inline Tag() : cov(-1), add(), rev() {}
        inline Tag(int x, int y, bool z) : cov(x), add(y), rev(z) {}
    } tag;
    inline void pull(const Tag &tp) {
        if (~tp.cov) { tag.cov = tp.cov; tag.add = 0; sum = (uint64_t)tp.cov * sz % MOD; }
        if (tp.add) { plus(tag.add, tp.add); plus(sum, (uint64_t)tp.add * sz % MOD); }
        if (tp.rev)  { std::swap(ch[0], ch[1]); tag.rev ^= 1; }
    }
} tr[N << 3];
inline Node& Node::operator()(bool c) { return tr[ch[c]]; }
inline void pushup(int u) { if (U.sz == 1) return; U.sz = U(L).sz + U(R).sz; plus(U.sum = U(L).sum, U(R).sum); }
namespace Data {
int bin[N << 3];
static size_t tot = 0, bintot = 0;
inline void recycle(int u) {
    if (u && --U.refCnt == 0)
        recycle(U[L]), recycle(U[R]), bin[++bintot] = u;
}
#define newnode (bintot ? bin[bintot--] : ++tot)
template<class... Args> inline int alloc(Args&&... args) {
    auto u = newnode;
    return tr[u] = Node(args...), pushup(u), u;
}
inline void check(int &u) {
    if (U.refCnt == 1) return;
    int tmp = newnode; tr[tmp] = tr[u];
    --U.refCnt;
    if (U.sz != 1) ++U(L).refCnt, ++U(R).refCnt;
    u = tmp, U.refCnt = 1;
}
#undef newnode
} // namespace Data
using Data::recycle; using Data::alloc; using Data::check;
inline void pushdown(int& u) {
    if (U.sz == 1) return;
    check(u);
    check(U[L]), U(L).pull(U.tag);
    check(U[R]), U(R).pull(U.tag);
    U.tag = Node::Tag();
}
inline std::pair<int,int> cut(int u) {
    if (U.sz == 1) return {0, 0};
    pushdown(u);
    ++U(L).refCnt, ++U(R).refCnt;
    std::pair<int,int> ret(U[L], U[R]);
    recycle(u);
    return ret;
}

// min(wl, wr) / wfa < alpha(1/4) 此处如果 l 更重就返回 true
inline bool isTooHeavy(int wl, int wr) { return wr * 3 < wl; }
/* Double Rotate: (example)
        u              u
        / \            / \
    3   1          /   \
    / \     ===>   2     2
    1   2          / \   / \
        / \        1   1 1   1
    1   1   */
inline int merge(int x, int y) {
    if (!x || !y) return x | y;
    if (isTooHeavy(tr[x].sz, tr[y].sz)) {
        auto [xl, xr] = cut(x);
        if (isTooHeavy(tr[xr].sz + tr[y].sz, tr[xl].sz)) {
            auto [xrl, xrr] = cut(xr);
            return alloc(alloc(xl, xrl), alloc(xrr, y));
        }
        return alloc(xl, alloc(xr, y));
    }
    if (isTooHeavy(tr[y].sz, tr[x].sz)) {
        auto [yl, yr] = cut(y);
        if (isTooHeavy(tr[yl].sz + tr[x].sz, tr[yr].sz)) {
            auto [yll, ylr] = cut(yl);
            return alloc(alloc(x, yll), alloc(ylr, yr));
        }
        return alloc(alloc(x, yl), yr);
    }
    return alloc(x, y);
}
inline void balance(int &u) {
    if (U.sz == 1 || !isTooHeavy(U(L).sz, U(R).sz) && !isTooHeavy(U(R).sz, U(L).sz)) return;
    auto [x, y] = cut(u);
    u = merge(x, y);
}

int root;
void debug_all(int &u = root, int dep = 0) {
    pushdown(u);
    for (int i = dep; i; i--) putchar(' ');
    if (U.sz == 1) printf("#%d: (%d) sz=%d ref=%d\n", u, U.sum, U.sz, U.refCnt);
    else {
        printf("#%d: %d sz=%d l%dr%d ref=%d\n", u, U.sum, U.sz, U[L], U[R], U.refCnt);
        debug_all(U[L], dep + 2);
        debug_all(U[R], dep + 2);
    }
}
void debug_real(int &u = root, int dep = 0) {
    for (int i = dep; i; i--) putchar(' ');
    if (U.sz == 1) printf("#%d: ((%d)) sz=%d ref=%d, l%d_r%d, tag={%d,%d,%c}\n", u, U.sum, U.sz, U.refCnt, U[L], U[R], U.tag.cov, U.tag.add, U.tag.rev ? 'r' : ' ');
    else {
        printf("#%d: [%d] sz=%d l%dr%d ref=%d, tag={%d,%d,%c}\n", u, U.sum, U.sz, U[L], U[R], U.refCnt, U.tag.cov, U.tag.add, U.tag.rev ? 'r' : ' ');
        debug_real(U[L], dep + 2);
        debug_real(U[R], dep + 2);
    }
}
#define debug_all debug_real

std::pair<int,int> split(int u, int ord) {
    if (!u) return {0, 0};
    if (ord == 0) return {0, u};
    if (U.sz == ord) return {u, 0};
    auto [l, r] = cut(u);
    if (tr[l].sz >= ord) {
        auto [ll, lr] = split(l, ord);
        return {ll, merge(lr, r)};
    }
    auto [rl, rr] = split(r, ord - tr[l].sz);
    return {merge(l, rl), rr};
}

namespace Utils {
using Fn_t = void(*) (int&, int&);
Fn_t fns[5] = {
    [](int &u, int &x) { U.pull({x, 0, false}); },
    [](int &u, int &x) { U.pull({-1, x, false}); },
    [](int &u, int & ) { U.pull({-1, 0, true}); },
    [](int &u, int &v) { ++U.refCnt, recycle(v), check(v = u); }, // Copy u to v
    [](int &u, int &v) { std::swap(u, v); }
};
void build(int l, int r, int& u = root) {
    if (l == r) u = alloc(a[l]);
    else {
        int mid = l + r >> 1;
        int ls, rs;
        build(l, mid, ls);
        build(mid + 1, r, rs);
        u = alloc(ls, rs);
    }
}
inline void modify(int srcL, int srcR, int destL, int destR, ProtoType tp) {
    ++srcL, ++destL, ++srcR, ++srcR, ++destR, ++destR;
    std::pair<int,int> rgs[2]{{destL, destR}, {srcL, srcR}};
    if (rgs[1] < rgs[0]) std::swap(rgs[0], rgs[1]);
    auto [tmp1, r] = split(root, rgs[1].second);
/* #ifdef DEBUG
    puts("==== tmp1 ===="), debug_all(tmp1);
#endif */
    auto [tmp2, u2] = split(tmp1, rgs[1].first);
/* #ifdef DEBUG
    puts("==== tmp2 ===="), debug_all(tmp2);
#endif */
    auto [tmp3, mid] = split(tmp2, rgs[0].second);
/* #ifdef DEBUG
    puts("==== tmp3 ===="), debug_all(tmp3);
#endif */
    auto [l, u1] = split(tmp3, rgs[0].first);
#ifdef DEBUG
    printf("==== ls ====\n"), debug_all(l);
    printf("==== u1 ====\n"), debug_all(u1);
    printf("==== md ====\n"), debug_all(mid);
    printf("==== u2 ====\n"), debug_all(u2);
    printf("==== rs ====\n"), debug_all(r);
    fflush(stdout);
#endif
    if (rgs->first == srcL) fns[tp](u1, u2);
    else fns[tp](u2, u1);
    #ifdef DEBUG
        printf("==== u1 (after) ====\n"), debug_all(u1);
        printf("==== u2 (after) ====\n"), debug_all(u2);
        fflush(stdout);
    #endif
    root = merge(merge(merge(l, u1), mid), merge(u2, r));
}
inline void modify(int rgL, int rgR, ProtoType tp, int x) {
    ++rgL, ++rgR, ++rgR;
    auto [tmp, r] = split(root, rgR);
#ifdef DEBUG
    printf("==== tmp ====\n"), debug_all(tmp);
#endif
    auto [l, u] = split(tmp, rgL);
#ifdef DEBUG
    printf("====== l ======\n"), debug_all(l);
    printf("====== u ======\n"), debug_all(u);
    printf("====== r ======\n"), debug_all(r);
#endif
    fns[tp](u, x);
#ifdef DEBUG
    printf("====== u(after) ======\n"), debug_all(u);
#endif
    root = merge(merge(l, u), r);
}
inline int querySum(int rgL, int rgR) {
    ++rgL, ++rgR, ++rgR;
    auto [tmp, r] = split(root, rgR);
    auto [l, u] = split(tmp, rgL);
    int res = U.sum;
    root = merge(merge(l, u), r);
    return res;
}
inline void extract(std::vector<int> &vec, int &u = root) {
    if (U.sz == 1) vec.push_back(U.sum);
    else {
        pushdown(u);
        extract(vec, U[L]);
        extract(vec, U[R]);
    }
}
} // namespace Utils
} // namespace WBLT
using namespace WBLT::Utils;
int main() {
    // freopen("P5350.out", "w", stdout);
    int n, m;
    fin >> n >> m;
    for (int i = 2; i <= n+1; i++) fin >> a[i];
    build(0, n + 3);
#ifdef DEBUG
    WBLT::debug_all();
#endif
    for (int opt, a, b, c, d, last = 0, i = 1; i <= m; i++) {
        fin >> opt >> a >> b;
        if (opt >= 2 && opt <= 5) fin >> c;
        if (opt == 4 || opt == 5) fin >> d;
        a ^= last, b ^= last, c ^= last, d ^= last;
        a %= MOD, b %= MOD, c %= MOD, d %= MOD;
        switch (opt) {
        case 1: fout << (last = querySum(a, b)) << '\n'; break;
        case 2: modify(a, b, COV, c); break;
        case 3: modify(a, b, ADD, c); break;
        case 6: modify(a, b, FLP, c); break;
        case 4: modify(a, b, c, d, COPY); break;
        case 5: modify(a, b, c, d, SWAP); break;
        }
#ifdef DEBUG
        switch (opt) {
        case 1: printf("@@@@@@@@@@@@@@ QuerySum [%d, %d] @@@@@@@@@@@@@@\n", a, b); break;
        case 2: printf("@@@@@@@@@@@@@@ Cover [%d, %d] ==> %d @@@@@@@@@@@@@@\n", a, b, c); break;
        case 3: printf("@@@@@@@@@@@@@@ Add [%d, %d] += %d @@@@@@@@@@@@@@\n", a, b, c); break;
        case 6: printf("@@@@@@@@@@@@@@ Flip [%d, %d] @@@@@@@@@@@@@@\n", a, b); break;
        case 4: printf("@@@@@@@@@@@@@@ Copy [%d, %d] to [%d, %d] @@@@@@@@@@@@@@\n", a, b, c, d); break;
        case 5: printf("@@@@@@@@@@@@@@ SWAP [%d, %d] and [%d, %d] @@@@@@@@@@@@@@\n", a, b, c, d); break;
        }
        WBLT::debug_all(); fflush(stdout);
#endif
    }
    std::vector<int> ans;
    ans.reserve(n + 4);
    extract(ans);
    for (int i = 2; i <= n+1; i++)
        fout << ans[i] << ' ';
    fout << '\n';
    return 0;
}
