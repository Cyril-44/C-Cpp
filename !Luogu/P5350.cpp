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
inline void Add(int &x, int y) { if ((x += y) >= MOD) x -= MOD; }
struct Node {
    friend inline void addRef(Node* u) { u && ++u->refCnt; }
    friend inline void delRef(Node* u) { if (u && --u->refCnt == 0) delete u; }
    friend inline void clone(Node*& u) {
        if (u->refCnt == 1) return;
        auto ptr = new Node(*u);
        --u->refCnt; ptr->refCnt = 1;
        addRef(u->ls), addRef(u->rs);
        u = ptr;
    }
    Node *ls, *rs;
    decltype(rng()) prio;
    int val, sum, sz;
    int cov, add;
    int refCnt;
    bool rev;
    inline Node(int x = 0) : ls(), rs(), prio(rng()), val(x), sum(x), sz(1), cov(-1), add(), refCnt(1), rev() {}
    inline ~Node() { delRef(ls); delRef(rs); }
    inline void pushup() {
        sz = (ls ? ls->sz : 0) + 1 + (rs ? rs->sz : 0);
        sum = val;
        if (ls) Add(sum, ls->sum);
        if (rs) Add(sum, rs->sum);
    }
    inline void pushdown() {
        if (ls) { clone(ls); if (rev) ls->flip(); ls->cover(cov), ls->plus(add); }
        if (rs) { clone(rs); if (rev) rs->flip(); rs->cover(cov), rs->plus(add); }
        cov = -1, add = 0, rev = false;
    }
    inline void cover(int x) {
        if (x == -1) return;
        add = 0, val = cov = x;
        sum = 1ull * x * sz % MOD;
    }
    inline void plus(int x) {
        if (!x) return;
        Add(add, x), Add(val, x);
        Add(sum, 1ull * x * sz % MOD);
    }
    inline void flip() {
        std::swap(ls, rs);
        rev ^= 1;
    }
    inline void setL(Node* x) { ls = x; pushup(); }
    inline void setR(Node* x) { rs = x; pushup(); }
};
using Fn_t1 = void(*) (Node*, int);
using Fn_t2 = void(*) (Node*&, Node*&);
Node* rt;
void debug_all(Node *u = rt, int dep = 0) {
    if (!u) return;
    u->pushdown();
    debug_all(u->ls, dep + 2);
    printf("#%04llx %2d: ", (unsigned long long)u & 0xFFFF, u->val);
    for (int i = dep; i--; putchar(' '));
    printf("sum=%d, sz=%d, refs=%d, prio_%lu, ls_%04llx rs_%04llx\n", u->sum, u->sz, u->refCnt, u->prio, (unsigned long long)u->ls & 0xFFFF, (unsigned long long)u->rs & 0xFFFF);
    debug_all(u->rs, dep + 2);
}
void* _fhq_treap_initialization = []() {
    auto l = new Node(), r = new Node();
    if (l->prio < r->prio) (rt = l)->rs = r;
    else (rt = r)->ls = l;
    rt->pushup();
    return nullptr;
}();
std::pair<Node*,Node*> splitByOrd(Node *u, int ord) {
    if (!u) return {0,0};
    clone(u);
    u->pushdown();
    int lsz = u->ls ? u->ls->sz : 0;
    if (lsz + 1 == ord) {
        auto rs = u->rs;
        u->setR(nullptr);
        return {u, rs};
    }
    if (lsz < ord) {
        auto [ls, rs] = splitByOrd(u->rs, ord - lsz - 1);
        u->setR(ls);
        return {u, rs};
    }
    auto [ls, rs] = splitByOrd(u->ls, ord);
    u->setL(rs);
    return {ls, u};
}
Node* merge(Node* l, Node* r) {
    if (!l) return r;
    if (!r) return l;
    clone(l); clone(r);
    l->pushdown();
    r->pushdown();
    if (l->sz * 1ull * rng() > r->sz * 1ull * rng()) return l->setR(merge(l->rs, r)), l;
    return r->setL(merge(l, r->ls)), r;
}
Fn_t1 oneRangeFuncs[3]{
    [](Node *u, int x){ u->plus(x); },
    [](Node *u, int x){ u->cover(x); },
    [](Node *u, int){ u->flip(); }
};
Fn_t2 twoRangeFuncs[2]{
    [](Node *&dest, Node *&src){ ++src->refCnt; delRef(dest); clone(dest = src); },
    [](Node *&dest, Node *&src){ std::swap(dest, src); }
};
namespace Utils {
inline void push_back(int x) {
    auto [ls, rs] = splitByOrd(rt, rt->sz - 1);
    rt = merge(merge(ls, new Node(x)), rs);
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
    /* printf("==== ls ====\n"), debug_all(ls);
    printf("==== u1 ====\n"), debug_all(u1);
    printf("==== md ====\n"), debug_all(mid);
    printf("==== u2 ====\n"), debug_all(u2);
    printf("==== rs ====\n"), debug_all(rs);
    printf("==== [%d, %d] ==== [%d, %d] ====\n", sorted[0].first, sorted[0].second, sorted[1].first, sorted[1].second);
    fflush(stdout); */
    rt = merge(merge(merge(ls, u1), mid), merge(u2, rs));
}
inline void modify(int l, int r, ProtoType1 tp, int x = 0) {
    auto [mid, rs] = splitByOrd(rt, ++r);
    auto [ls, u] = splitByOrd(mid, l);
    /* printf("====== ls ======\n"), debug_all(ls);
    printf("====== u =======\n"), debug_all(u);
    printf("====== rs ======\n"), debug_all(rs); */
    oneRangeFuncs[tp](u, x);
    rt = merge(merge(ls, u), rs);
}
inline int querySum(int l, int r) {
    auto [mid, rs] = splitByOrd(rt, ++r);
    auto [ls, u] = splitByOrd(mid, l);
    auto res = u->sum;
    rt = merge(merge(ls, u), rs);
    return res;
}
void extract(std::vector<int>& arr, Node *u = rt) {
    if (!u) return;
    u->pushdown();
    extract(arr, u->ls);
    arr.push_back(u->val);
    extract(arr, u->rs);
}
}
#undef U
}
using namespace FHQ_Treap::Utils;
int main() {
    // freopen("P5350.out", "w", stdout);
    int n, m;
    fin >> n >> m;
    for (int i = 1, x; i <= n; i++) {
        fin >> x;
        push_back(x);
    }
    // FHQ_Treap::debug_all();
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
        /* switch (opt) {
        case 1: printf("@@@@@@@@@@@@@@ QuerySum [%d, %d] @@@@@@@@@@@@@@\n", a, b); break;
        case 2: printf("@@@@@@@@@@@@@@ Cover [%d, %d] ==> %d @@@@@@@@@@@@@@\n", a, b, c); break;
        case 3: printf("@@@@@@@@@@@@@@ Add [%d, %d] += %d @@@@@@@@@@@@@@\n", a, b, c); break;
        case 6: printf("@@@@@@@@@@@@@@ Flip [%d, %d] @@@@@@@@@@@@@@\n", a, b); break;
        case 4: printf("@@@@@@@@@@@@@@ Copy [%d, %d] to [%d, %d] @@@@@@@@@@@@@@\n", a, b, c, d); break;
        case 5: printf("@@@@@@@@@@@@@@ SWAP [%d, %d] and [%d, %d] @@@@@@@@@@@@@@\n", a, b, c, d); break;
        }
        FHQ_Treap::debug_all(); fflush(stdout); */
    }
    std::vector<int> ans;
    ans.reserve(n + 2);
    extract(ans);
    for (int i = 1; i <= n; i++)
        fout << ans[i] << ' ';
    fout << '\n';
    // delete FHQ_Treap::rt;
    return 0;
}