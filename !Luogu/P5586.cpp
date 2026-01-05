#include <array>
#include <cfloat>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <random>
#include <type_traits>
#include <cassert>
constexpr int N = 300005;
constexpr unsigned MOD = (unsigned)1e9 + 7;
int rt[N];
enum ProtoType1 { ADD, COV, FLP };
enum ProtoType2 { COPY, SWAP };
std::mt19937 rng(std::random_device{}());
#define Inline __attribute__((always_inline)) inline
class FastIS{static constexpr size_t h=1<<20;char o[h];char*t,*v;FILE*K;public:FastIS(FILE*B=stdin):t(nullptr),v(nullptr),K(B){}~FastIS(){fclose(K);}Inline int get(){return(t==v)&&(v=(t=o)+fread(o,1,h,K),t==v)?EOF:*t++;}template<typename j>typename std::enable_if<std::is_unsigned<j>::value&&std::is_integral<j>::value,FastIS&>::type inline operator>>(j&d){int a=get();while(~a&&(a<'0'||a>'9'))a=get();for(d=0;a>='0'&&a<='9';a=get())d=(d<<3)+(d<<1)+(a^'0');return*this;}template<typename j>typename std::enable_if<std::is_signed<j>::value&&std::is_integral<j>::value&&!std::is_same<j,bool>::value&&!std::is_same<j,char>::value,FastIS&>::type inline operator>>(j&d){int a=get();bool L=false;while(~a&&(a<'0'||a>'9')&&(a^'-'))a=get();if(a=='-')a=get(),L=true;for(d=0;a>='0'&&a<='9';a=get())d=(d<<3)+(d<<1)+(a^'0');d=L?-d:d;return*this;}template<typename j>typename std::enable_if<std::is_floating_point<j>::value,FastIS&>::type inline operator>>(j&d){int a=get();bool L=false;while(~a&&(a<'0'||a>'9')&&(a^'-'))a=get();if(a=='-')a=get(),L=true;using G=typename std::conditional<std::is_same<j,float>::value,std::uint32_t,std::uint64_t>::type;G I;for(I=0;a>='0'&&a<='9';a=get())I=(I<<3)+(I<<1)+(a^'0');d=I;if(a=='.'){G w=1;for(a=get();a>='0'&&a<='9';a=get())d+=(a^'0')*(1./(w*=10));}if(L)d=-d;return*this;}inline FastIS&operator>>(bool&U){int a=get();while(~a&&a!='0'&&a!='1')a=get();U=static_cast<bool>(a&1);return*this;}inline FastIS&operator>>(char&n){int a=get();while(a==' '||a=='\r'||a=='\n')a=get();n=a;return*this;}inline FastIS&operator>>(char*n){int a=get();while(~a&&(a==' '||a=='\r'||a=='\n'))a=get();while(~a&&(a!=' '&&a!='\r'&&a!='\n'))*n++=a,a=get();return*this;}}fin;class FastOS{static constexpr size_t h=1<<20;static constexpr double R=1e-9;char o[h],*O;const char*y;FILE*q;int l;public:FastOS(FILE*B=stdout):O(o),y(o+h),q(B),l(-1){setvbuf(q,nullptr,_IONBF,0);}~FastOS(){flush();fclose(q);}FastOS&flush(){fwrite(o,O-o,1,q);return*this;}FastOS&put(int H){*O++=H;if(O==y)fwrite(o,h,1,q),O=o;return*this;}FastOS&setprecision(const int&S){l=S;return*this;}template<typename j>typename std::enable_if<std::is_unsigned<j>::value&&std::is_integral<j>::value,FastOS&>::type inline operator<<(j d){static int n[32];n[*n=1]=d%10;while(d/=10)n[++(*n)]=d%10;while(*n)put(n[(*n)--]|'0');return*this;}template<typename j>typename std::enable_if<std::is_signed<j>::value&&std::is_integral<j>::value&&!std::is_same<j,char>::value,FastOS&>::type inline operator<<(const j&d){if(d<0)put('-')<<(typename std::make_unsigned<j>::type)(-d);else*this<<(typename std::make_unsigned<j>::type)(d);return *this;}template<typename j>typename std::enable_if<std::is_floating_point<j>::value,FastOS&>::type inline operator<<(j d){if(d<0)put('-'),d=-d;using G=typename std::conditional<std::is_same<j,float>::value,std::uint32_t,std::uint64_t>::type;G z=static_cast<G>(d);j O=d-z;*this<<z;if(l>0||l<0&&O>R){put('.');for(int J=0;l>0&&J<l||l<0&&O>R;J++){int N=static_cast<int>(O*=10);put(N|'0');O-=N;}}return*this;}FastOS&operator<<(const char&H){put(H);return*this;}}fout;
namespace WBLT {
#define U tr[u]
#define UL tr[U.ls]
#define UR tr[U.rs]
inline void plus(int &x, int y) { if ((x += y) >= MOD) x -= MOD; }
inline void plus(int &x, int y, int z) { plus(x, y); plus(x, z); }
enum SonType {Ls, Rs};
struct Node {
    int ls, rs;
    int sz;
    int dat, sum;
    Node (int v = 0) : dat(v), sum(v) {}
    struct Tag {
        int cov, add; bool rev;
        inline Tag() : cov(-1), add(), rev() {}
    } tag;
    inline void pull(const Tag &tp) {
        if (~tp.cov) { dat = tag.cov = tp.cov; sum = (uint64_t)tp.cov * sz % MOD; }
        if (!tp.add) { plus(dat, tp.add); plus(tag.add, tp.add); plus(sum, (uint64_t)tp.add * sz % MOD); }
        if (tp.rev)  { std::swap(ls, rs); tag.rev ^= 1; }
    }
} tr[N << 4];
inline void pushup(int u) { U.sz = UL.sz + 1 + UR.sz; plus(U.sum = U.dat, UL.sum, UR.sum); }
inline void pushdown(int u) {
    if (U.ls) UL.pull(U.tag);
    if (U.rs) UR.pull(U.tag);
    U.tag = Node::Tag();
}
inline bool isTooHeavy(int u) {
    if (!U.ls)
    if (std::min(tr[u]))
}
namespace Rotate {
    inline void rotate() {

    }
    inline void merge() {

    }z
    inline void balance() {

    }
}
namespace Non_Rotate {
    inline void merge() {

    }
}
inline void merge() {
    
}
}
using namespace WBLT::Utils;
int main() {
    // freopen("P5350.out", "w", stdout);
    int n, m;
    fin >> n >> m;
    for (int i = 1, x; i <= n; i++) {
        fin >> x;
        push_back(x);
    }
#ifdef DEBUG
    FHQ_Treap::debug_all();
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
