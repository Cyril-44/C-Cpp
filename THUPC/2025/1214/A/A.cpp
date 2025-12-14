#include <bits/stdc++.h>
#include <initializer_list>
// #define LUOGU
#if defined(ONLINE_JUDGE) && !defined(LUOGU)
# pragma GCC optimize(2, 3, "inline", "unroll-loops", "fast-math", "inline-small-functions", "no-stack-protector", "delete-null-pointer-checks")
# pragma GCC target("tune=native")
#endif
#define Inline __attribute__((always_inline)) inline
#define For(i, s, t) for (int i = (s); i <= (t); ++i)
#define Forv(i, s, t, ...) for (int i = (s), __VA_ARGS__; i <= (t); ++i)
#define roF(i, t, s) for (int i = (t); i >= (s); --i)
#define roFv(i, t, s, ...) for (int i = (t), __VA_ARGS__; i >= (s); --i)
#define Rep(c) for (int tempFor_count = c; tempFor_count; --tempFor_count)
#define Repv(c, ...) for (int tempFor_count = c, __VA_ARGS__; tempFor_count; --tempFor_count)
#define umax(x, y) ((x) < (y) ? (x) = (y) : (x))
using namespace std;using pii_t=pair<int,int>;using pll_t=pair<int64_t,int64_t>;using veci_t=vector<int>;using vecl_t=vector<int64_t>;Inline int Popcnt(int x){return __builtin_popcount((unsigned)x);}Inline int Popcnt(unsigned x){return __builtin_popcount(x);}Inline int Popcnt(int64_t x){return __builtin_popcountll((uint64_t)x);}Inline int Popcnt(uint64_t x){return __builtin_popcountll(x);}Inline int Log2(int x){return 31-__builtin_clz((unsigned)x|1);}Inline int Log2(unsigned x){return 31-__builtin_clz(x|1);}Inline int Log2(int64_t x){return 63-__builtin_clzll((uint64_t)x|1);}Inline int Log2(uint64_t x){return 63-__builtin_clzll(x|1);}
class FastIS{static constexpr size_t P=1<<20;char r[P];char*u,*G;FILE*I;public:FastIS(FILE*Y=stdin):u(nullptr),G(nullptr),I(Y){}__attribute__((always_inline))inline int get(){return(u==G)&&(G=(u=r)+fread(r,1,P,I),u==G)?EOF:*u++;}template<typename A,typename=typename std::enable_if<std::is_arithmetic<A>::value>::type>FastIS&operator>>(A&j){if _GLIBCXX17_CONSTEXPR(std::is_same<A,bool>::value){int a=get();while(~a&&a!=(char)(48)&&a!=(char)(49))a=get();j=static_cast<bool>(a&1);}else if _GLIBCXX17_CONSTEXPR(std::is_same<A,char>::value){j=get();while(j==(char)(32)||j==(char)(13)||j==(char)(10))j=get();}else if _GLIBCXX17_CONSTEXPR(std::is_integral<A>::value){if _GLIBCXX17_CONSTEXPR(std::is_unsigned<A>::value){int a=get();while(~a&&(a<(char)(48)||a>(char)(57)))a=get();for(j=0;a>=(char)(48)&&a<=(char)(57);a=get())j=(j<<3)+(j<<1)+(a^(char)(48));}else{int a=get();bool m=false;while(~a&&(a<(char)(48)||a>(char)(57))&&(a^(char)(45)))a=get();if(a==(char)(45))a=get(),m=true;for(j=0;a>=(char)(48)&&a<=(char)(57);a=get())j=(j<<3)+(j<<1)+(a^(char)(48));j=m?-j:j;}}else if _GLIBCXX17_CONSTEXPR(std::is_floating_point<A>::value){int a=get();bool m=false;while(~a&&(a<(char)(48)||a>(char)(57))&&(a^(char)(45)))a=get();if(a==(char)(45))a=get(),m=true;using E=typename std::conditional<std::is_same<A,float>::value,std::uint32_t,std::uint64_t>::type;E Q;for(Q=0;a>=(char)(48)&&a<=(char)(57);a=get())Q=(Q<<3)+(Q<<1)+(a^(char)(48));j=Q;if(a==(char)(46)){E a=1;for(a=get();a>=(char)(48)&&a<=(char)(57);a=get())j+=(a^(char)(48))*(1./(a*=10));}if(m)j=-j;}else throw;return*this;}FastIS&operator>>(char*i){int a=get();while(~a&&(a==(char)(32)||a==(char)(13)||a==(char)(10)))a=get();while(~a&&(a!=(char)(32)&&a!=(char)(13)&&a!=(char)(10)))*i++=a,a=get();return*this;}}fin;class FastOS{static constexpr size_t P=1<<20;static constexpr double x=1e-9;char r[P],*b;const char*l;FILE*w;int o;public:FastOS(FILE*Y=stdout):b(r),l(r+P),w(Y),o(-1){setvbuf(w,nullptr,_IONBF,0);}~FastOS(){flush();fclose(w);}FastOS&flush(){fwrite(r,b-r,1,w);return*this;}FastOS&put(int g){*b++=g;if(b==l)fwrite(r,P,1,w),b=r;return*this;}FastOS&setprecision(const int&t){o=t;return*this;}template<typename a,typename=typename std::enable_if<std::is_arithmetic<a>::value>::type>FastOS&operator<<(a j){if _GLIBCXX17_CONSTEXPR(std::is_same<a,char>::value)put(j);else if _GLIBCXX17_CONSTEXPR(std::is_integral<a>::value){if _GLIBCXX17_CONSTEXPR(std::is_signed<a>::value)if(j<0)put((char)(45)),j=-j;static uint8_t i[40];i[*i=1]=j%10;while(j/=10)i[++(*i)]=j%10;while(*i)put(i[(*i)--]|(char)(48));}else if _GLIBCXX17_CONSTEXPR(std::is_floating_point<a>::value){if(j<0)put((char)(45)),j=-j;using E=typename std::conditional<std::is_same<a,float>::value,std::uint32_t,std::uint64_t>::type;E v=static_cast<E>(j);a B=j-v;*this<<v;if(o>0||(o<0&&B>x)){put((char)(46));for(int R=0;(o>0&&R<o)||(o<0&&B>x);R++){int X=static_cast<int>(B*=10);put(X|(char)(48));B-=X;}}}else throw;return*this;}FastOS&operator<<(char*i){while(*i)put(*i++);return*this;}FastOS&operator<<(const std::string&s){return*this<<s.c_str();}}fout;

// #define MULTI_TEST_CASES

constexpr int N = 500005;
vector<int> g[N];
struct Query{
    int l, r, u, bl, br, id;
    inline bool operator<(const Query &rhs) const {
        return l < rhs.l || l == rhs.l && r < rhs.r;
    }
} qs[N];
struct SegTr {
#define Ls(u) tr[u].ls
#define Rs(u) tr[u].rs
#define tls(u) tr[Ls(u)]
#define trs(u) tr[Rs(u)]
struct Node {
    int l, r;
    int ls, rs;
    int mx, tag;
} tr[N << 2];
int tot;
inline void pushup(int u) {
    umax(tls(u).mx, trs(u).mx);
}
inline void pushdown(int u) {
    if (tr[u].tag) {
        if (Ls(u)) {
            umax(tr[Ls(u)].mx, tr[u].tag);
            umax(tr[Ls(u)].tag, tr[u].tag);
        }
        if (Rs(u)) {
            umax(tr[Rs(u)].mx, tr[u].tag);
            umax(tr[Rs(u)].tag, tr[u].tag);
        }
        tr[u].tag = 0;
    }
}
void build(int &u, int l, int r) {
    tr[u = ++tot] = {l, r};
    if (l == r) return;
    int mid = l + r >> 1;
    build(Ls(u), l, mid);
    build(Rs(u), mid+1, r);
}
int qL, qR, X;
int que(int u) {
    if (!u) return 0;
    if (qL <= tr[u].l && tr[u].r <= qR) return tr[u].mx;
    pushdown(u); int res = 0;
    if (qL <= tls(u).r) res = max(res, que(Ls(u)));
    if (trs(u).l <= qR) res = max(res, que(Rs(u)));
    return res;
}
void upd(int u) {
    if (!u)
        return;
    if (qL <= tr[u].l && tr[u].r <= qR) {
        umax(tr[u].mx, X);
        umax(tr[u].tag, X);
    }
    pushdown(u);
    if (qL <= tls(u).r) upd(Ls(u));
    if (trs(u).l <= qR) upd(Rs(u));
    pushup(u);
}
inline int getmx(int u, int l, int r) {
    qL = l, qR = r;
    return que(u);
}
inline void update(int u, int l, int r, int x) {
    X = x, qL = l, qR = r;
    upd(u);
}
} init, seg, tmp;
namespace HLD {
int top[N], ts;
int dfn[N], dep[N];
int hson[N], sz[N];
int root[N], fa[N];
void dfs1(int u, int pre) {
    sz[u] = 1, hson[u] = 0;
    for (int v : g[u]) {
        if (v == pre) continue;
        dep[v] = dep[u] + 1;
        fa[v] = u;
        dfs1(v, u);
        sz[u] += sz[v];
        if (sz[v] > sz[hson[u]])
            hson[u] = v;
    }
}
void dfs2(int u, int pre, int tp) {
    dfn[u] = ++ts;
    top[u] = tp;
    if (hson[u]) dfs2(hson[u], u, tp);
    else init.build(root[tp], dfn[tp], dfn[u]);
    for (int v : g[u]) {
        if (v == pre || v == hson[u]) continue;
        dfs2(v, u, v);
    }
}
inline void update(int u, SegTr& seg) {
    while (u) {
        seg.update(root[top[u]], dfn[top[u]], dfn[u], u);
        u = fa[top[u]];
    }
}
inline int query(int u, SegTr& seg) {
    int res = 0;
    while (u) {
        res = max(res, seg.getmx(root[top[u]], dfn[top[u]], dfn[u]));
        u = fa[top[u]];
    }
    return res;
}
#undef Ls
#undef Rs
#undef tls
#undef trs
}
int a[N], ans[N];
inline void solveSingleTestCase() {
    int n, m, q;
    fin >> n >> m >> q;
    For (i, 1, n) g[i].clear();
    Repv (n-1, u, v) {
        fin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    HLD::dfs1(1, 0), HLD::dfs2(1, 0, 1);
    For (i, 1, m) fin >> a[i];
    int B = floor(sqrt(q));
    For (i, 1, q) {
        fin >> qs[i].l >> qs[i].r >> qs[i].u;
        qs[i].id = i;
        qs[i].bl = (qs[i].l + B - 1) / B;
        qs[i].br = (qs[i].r + B - 1) / B;
    }
    std::sort(qs+1, qs+1 + q);
    int last_blk = 0, right = 0;
    For (i, 1, q) {
        int &ans = ::ans[qs[i].id];
        if (last_blk != qs[i].bl) seg = init, last_blk = qs[i].bl, right = qs[i].bl * B;
        if (qs[i].bl == qs[i].br) {
            For (j, qs[i].l, qs[i].r)
                HLD::update(a[j], seg);
            ans = HLD::query(qs[i].u, seg);
            seg = init;
            continue;
        }
        while (right < qs[i].r) HLD::update(a[++right], seg);
        tmp = seg;
        For (j, qs[i].l, qs[i].bl * B)
            HLD::update(a[j], tmp);
        ans = HLD::query(qs[i].u, tmp);
    }
    For (i, 1, q) fout << ans[i] << '\n';
}

int main() {
    int testCases = 1;
#ifdef MULTI_TEST_CASES
    fin >> testCases;
#endif
    while (testCases--) solveSingleTestCase();
    return 0;
}