#include <array>
#include <cfloat>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <random>
#include <type_traits>
constexpr int N = 300005, MOD = (int)1e9 + 7;
int rt[N];
enum ProtoType1 { ADD, COV, FLP };
enum ProtoType2 { COPY, SWAP };
std::mt19937 rng(std::random_device{}());
#define Inline __attribute__((always_inline)) inline
class FastIS{static constexpr size_t h=1<<20;char o[h];char*t,*v;FILE*K;public:FastIS(FILE*B=stdin):t(nullptr),v(nullptr),K(B){}~FastIS(){fclose(K);}Inline int get(){return(t==v)&&(v=(t=o)+fread(o,1,h,K),t==v)?EOF:*t++;}template<typename j>typename std::enable_if<std::is_unsigned<j>::value&&std::is_integral<j>::value,FastIS&>::type inline operator>>(j&d){int a=get();while(~a&&(a<'0'||a>'9'))a=get();for(d=0;a>='0'&&a<='9';a=get())d=(d<<3)+(d<<1)+(a^'0');return*this;}template<typename j>typename std::enable_if<std::is_signed<j>::value&&std::is_integral<j>::value&&!std::is_same<j,bool>::value&&!std::is_same<j,char>::value,FastIS&>::type inline operator>>(j&d){int a=get();bool L=false;while(~a&&(a<'0'||a>'9')&&(a^'-'))a=get();if(a=='-')a=get(),L=true;for(d=0;a>='0'&&a<='9';a=get())d=(d<<3)+(d<<1)+(a^'0');d=L?-d:d;return*this;}template<typename j>typename std::enable_if<std::is_floating_point<j>::value,FastIS&>::type inline operator>>(j&d){int a=get();bool L=false;while(~a&&(a<'0'||a>'9')&&(a^'-'))a=get();if(a=='-')a=get(),L=true;using G=typename std::conditional<std::is_same<j,float>::value,std::uint32_t,std::uint64_t>::type;G I;for(I=0;a>='0'&&a<='9';a=get())I=(I<<3)+(I<<1)+(a^'0');d=I;if(a=='.'){G w=1;for(a=get();a>='0'&&a<='9';a=get())d+=(a^'0')*(1./(w*=10));}if(L)d=-d;return*this;}inline FastIS&operator>>(bool&U){int a=get();while(~a&&a!='0'&&a!='1')a=get();U=static_cast<bool>(a&1);return*this;}inline FastIS&operator>>(char&n){int a=get();while(a==' '||a=='\r'||a=='\n')a=get();n=a;return*this;}inline FastIS&operator>>(char*n){int a=get();while(~a&&(a==' '||a=='\r'||a=='\n'))a=get();while(~a&&(a!=' '&&a!='\r'&&a!='\n'))*n++=a,a=get();return*this;}}fin;class FastOS{static constexpr size_t h=1<<20;static constexpr double R=1e-9;char o[h],*O;const char*y;FILE*q;int l;public:FastOS(FILE*B=stdout):O(o),y(o+h),q(B),l(-1){setvbuf(q,nullptr,_IONBF,0);}~FastOS(){flush();fclose(q);}FastOS&flush(){fwrite(o,O-o,1,q);return*this;}FastOS&put(int H){*O++=H;if(O==y)fwrite(o,h,1,q),O=o;return*this;}FastOS&setprecision(const int&S){l=S;return*this;}template<typename j>typename std::enable_if<std::is_unsigned<j>::value&&std::is_integral<j>::value,FastOS&>::type inline operator<<(j d){static int n[32];n[*n=1]=d%10;while(d/=10)n[++(*n)]=d%10;while(*n)put(n[(*n)--]|'0');return*this;}template<typename j>typename std::enable_if<std::is_signed<j>::value&&std::is_integral<j>::value&&!std::is_same<j,char>::value,FastOS&>::type inline operator<<(const j&d){if(d<0)put('-')<<(typename std::make_unsigned<j>::type)(-d);else*this<<(typename std::make_unsigned<j>::type)(d);return *this;}template<typename j>typename std::enable_if<std::is_floating_point<j>::value,FastOS&>::type inline operator<<(j d){if(d<0)put('-'),d=-d;using G=typename std::conditional<std::is_same<j,float>::value,std::uint32_t,std::uint64_t>::type;G z=static_cast<G>(d);j O=d-z;*this<<z;if(l>0||l<0&&O>R){put('.');for(int J=0;l>0&&J<l||l<0&&O>R;J++){int N=static_cast<int>(O*=10);put(N|'0');O-=N;}}return*this;}FastOS&operator<<(const char&H){put(H);return*this;}}fout;
namespace FHQ_Treap {
#define U tr[u]
#define UL tr[U.ls]
#define UR tr[U.rs]
inline void Add(int &x, int y) { if ((x += y) >= MOD) x -= MOD; }
struct Node {
    decltype(rng()) prio;
    int ls, rs, val, sum, sz, cov, add, refCnt;
    bool rev;
    inline Node(int x = 0) : prio(rng()), ls(), rs(), val(x), sum(x), sz(1), cov(-1), add(), refCnt(1), rev() {}
    inline void cover(int x) { if (~x) add = 0, val = cov = x, sum = 1ull * x * sz % MOD; }
    inline void plus(int x) { if (x) Add(add, x), Add(val, x), Add(sum, 1ull * x * sz % MOD); }
    inline void flip() { std::swap(ls, rs); rev ^= 1; }
} tr[N << 3];
using Fn_t1 = void(*) (int, int);
using Fn_t2 = void(*) (int&, int&);

namespace Data {
    int bin[N << 3], bintop = 0, tot = 0;
    template<typename... Args>
    inline int alloc(Args... args) {
        if (bintop) { tr[bin[bintop]] = std::move(Node(args...)); return bin[bintop--]; }
        tr[++tot] = Node(args...); return tot;
    }
    inline void recycle(int u) {
        if (u && --U.refCnt == 0)
            recycle(U.ls), recycle(U.rs), bin[++bintop] = u;
    }
}
using Data::alloc; using Data::recycle;

inline void addRef(int u) { u && ++U.refCnt; }
inline void delRef(int u) { if (u && --U.refCnt == 0) recycle(u); }
inline void clone(int &u) {
    if (U.refCnt == 1) return;
    int v = alloc(U);
    --U.refCnt; tr[v].refCnt = 1;
    addRef(U.ls), addRef(U.rs);
}

inline void pushup(int u) {
    U.sz = UL.sz + 1 + UR.sz;
    Add(U.sum = U.val, UL.sum), Add(U.sum, UR.sum);
}
inline void pushdown(int u) {
    if (U.ls) { clone(U.ls); if (U.rev) UL.flip(); UL.cover(U.cov), UL.plus(U.add); }
    if (U.rs) { clone(U.rs); if (U.rev) UR.flip(); UR.cover(U.cov), UR.plus(U.add); }
    U.cov = -1, U.add = 0, U.rev = false;
}

int rt;
void debug_all(int u = rt, int dep = 0) {
    if (!u) return;
    pushdown(u);
    debug_all(U.ls, dep + 2);
    printf("#%02d %2d: ", u, U.val);
    for (int i = dep; i--; putchar(' '));
    printf("sum=%d, sz=%d, refs=%d, prio_%lu, ls_%02d rs_%02d\n", U.sum, U.sz, U.refCnt, U.prio, U.ls, U.rs);
    debug_all(U.rs, dep + 2);
}
void* _fhq_treap_initialization = []() {
    int l = alloc(), r = alloc();
    if (tr[l].prio < tr[r].prio) tr[rt = l].rs = r;
    else tr[rt = r].ls = l;
    pushup(rt);
    return nullptr;
}();
std::pair<int,int> splitByOrd(int u, int ord) {
    if (!u) return {0,0};
    clone(u); pushdown(u);
    if (UL.sz + 1 == ord) {
        auto rs = U.rs;
        U.rs = 0; pushup(u);
        return {u, rs};
    }
    if (UL.sz < ord) {
        auto [ls, rs] = splitByOrd(U.rs, ord - UL.sz - 1);
        U.rs = ls; pushup(u);
        return {u, rs};
    }
    auto [ls, rs] = splitByOrd(U.ls, ord);
    U.ls = rs; pushup(u);
    return {ls, u};
}
int merge(int l, int r) {
    if (!l || !r) return l | r;
    clone(l); pushdown(l);
    clone(r); pushdown(r);
    if (tr[l].prio < tr[r].prio) {
        tr[l].rs = merge(tr[l].rs, r);
        return pushup(l), l;
    }
    tr[r].ls = merge(l, tr[r].ls);
    return pushup(r), r;
}
Fn_t1 oneRangeFuncs[3]{
    [](int u, int x){ U.plus(x); },
    [](int u, int x){ U.cover(x); },
    [](int u, int){ U.flip(); }
};
Fn_t2 twoRangeFuncs[2]{
    [](int &dest, int &src){ ++tr[src].refCnt; delRef(dest); clone(dest = src); },
    [](int &dest, int &src){ std::swap(dest, src); }
};
namespace Utils {
inline void push_back(int x) {
    auto [ls, rs] = splitByOrd(rt, tr[rt].sz - 1);
    rt = merge(merge(ls, alloc(x)), rs);
}
inline void modify(int destL, int destR, int srcL, int srcR, ProtoType2 tp) {
    ++destR, ++srcR;
    std::array<std::pair<int,int>, 2> sorted{std::pair<int,int>{destL, destR}, std::pair<int,int>{srcL, srcR}};
    std::sort(sorted.begin(), sorted.end());
    auto [tmp1, rs] = splitByOrd(rt, sorted[1].second);
    auto [tmp2, u2] = splitByOrd(tmp1, sorted[1].first);
    auto [tmp3, mid] = splitByOrd(tmp2, sorted[0].second);
    auto [ls, u1] = splitByOrd(tmp3, sorted[0].first);
    if (std::make_pair(destL, destR) != sorted.front()) twoRangeFuncs[tp](u2, u1);
    else twoRangeFuncs[tp](u1, u2);
#ifdef DEBUG
    printf("==== ls ====\n"), debug_all(ls);
    printf("==== u1 ====\n"), debug_all(u1);
    printf("==== md ====\n"), debug_all(mid);
    printf("==== u2 ====\n"), debug_all(u2);
    printf("==== rs ====\n"), debug_all(rs);
    printf("==== [%d, %d] ==== [%d, %d] ====\n", sorted[0].first, sorted[0].second, sorted[1].first, sorted[1].second);
    fflush(stdout);
#endif
    rt = merge(merge(merge(ls, u1), mid), merge(u2, rs));
}
inline void modify(int l, int r, ProtoType1 tp, int x = 0) {
    auto [mid, rs] = splitByOrd(rt, ++r);
    auto [ls, u] = splitByOrd(mid, l);
#ifdef DEBUG
    printf("====== ls ======\n"), debug_all(ls);
    printf("====== u =======\n"), debug_all(u);
    printf("====== rs ======\n"), debug_all(rs);
#endif
    oneRangeFuncs[tp](u, x);
    rt = merge(merge(ls, u), rs);
}
inline int querySum(int l, int r) {
    auto [mid, rs] = splitByOrd(rt, ++r);
    auto [ls, u] = splitByOrd(mid, l);
    auto res = U.sum;
    rt = merge(merge(ls, u), rs);
    return res;
}
void extract(std::vector<int>& arr, int u = rt) {
    if (!u) return;
    pushdown(u);
    extract(arr, U.ls);
    arr.push_back(U.val);
    extract(arr, U.rs);
}
}
#undef U
#undef UL
#undef UR
}
using namespace FHQ_Treap::Utils;
int main() {
    // freopen("P5350.out", "w", stdout);
    int n, m;
    fin >> n >> m;
    for (int i = 1, x; i <= n; i++) {
        fin >> x;
        push_back(x);
    FHQ_Treap::debug_all();
    puts("=================\n");
    }
#ifdef DEBUG
    FHQ_Treap::debug_all();
#endif
    for (int opt, a, b, c, d, last = 0, i = 1; i <= m; i++) {
        fin >> opt >> a >> b;
        if (opt >= 2 && opt <= 5) fin >> c;
        if (opt == 4 || opt == 5) fin >> d;
        // a ^= last, b ^= last, c ^= last, d ^= last;
        a %= MOD, b %= MOD, c %= MOD, d %= MOD;
        switch (opt) {
        case 1: fout << (last = querySum(a, b)) << '\n'; break;
        case 2: modify(a, b, COV, c); break;
        case 3: modify(a, b, ADD, c); break;
        case 6: modify(a, b, FLP); break;
        case 4: modify(c, d, a, b, COPY); break;
        case 5: modify(c, d, a, b, SWAP); break;
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
        FHQ_Treap::debug_all(); fflush(stdout);
#endif
    }
    std::vector<int> ans;
    ans.reserve(n + 2);
    extract(ans);
    for (int i = 1; i <= n; i++)
        fout << ans[i] << ' ';
    fout << '\n';
    return 0;
}
