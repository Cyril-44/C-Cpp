#include <stdio.h>
#include <vector>
#include <array>
#include <algorithm>
constexpr int N = 1000005;
using LL = long long;
#include <cstdint>
#include <type_traits>
class FastIS{static constexpr size_t P=1<<20;char r[P];char*u,*G;FILE*I;public:FastIS(FILE*Y=stdin):u(nullptr),G(nullptr),I(Y){}__attribute__((always_inline))inline int get(){return(u==G)&&(G=(u=r)+fread(r,1,P,I),u==G)?EOF:*u++;}template<typename A,typename=typename std::enable_if<std::is_arithmetic<A>::value>::type>FastIS&operator>>(A&j){if _GLIBCXX17_CONSTEXPR(std::is_same<A,bool>::value){int a=get();while(~a&&a!=(char)(48)&&a!=(char)(49))a=get();j=static_cast<bool>(a&1);}else if _GLIBCXX17_CONSTEXPR(std::is_same<A,char>::value){j=get();while(j==(char)(32)||j==(char)(13)||j==(char)(10))j=get();}else if _GLIBCXX17_CONSTEXPR(std::is_integral<A>::value){if _GLIBCXX17_CONSTEXPR(std::is_unsigned<A>::value){int a=get();while(~a&&(a<(char)(48)||a>(char)(57)))a=get();for(j=0;a>=(char)(48)&&a<=(char)(57);a=get())j=(j<<3)+(j<<1)+(a^(char)(48));}else{int a=get();bool m=false;while(~a&&(a<(char)(48)||a>(char)(57))&&(a^(char)(45)))a=get();if(a==(char)(45))a=get(),m=true;for(j=0;a>=(char)(48)&&a<=(char)(57);a=get())j=(j<<3)+(j<<1)+(a^(char)(48));j=m?-j:j;}}else if _GLIBCXX17_CONSTEXPR(std::is_floating_point<A>::value){int a=get();bool m=false;while(~a&&(a<(char)(48)||a>(char)(57))&&(a^(char)(45)))a=get();if(a==(char)(45))a=get(),m=true;using E=typename std::conditional<std::is_same<A,float>::value,std::uint32_t,std::uint64_t>::type;E Q;for(Q=0;a>=(char)(48)&&a<=(char)(57);a=get())Q=(Q<<3)+(Q<<1)+(a^(char)(48));j=Q;if(a==(char)(46)){E a=1;for(a=get();a>=(char)(48)&&a<=(char)(57);a=get())j+=(a^(char)(48))*(1./(a*=10));}if(m)j=-j;}else{throw;}return*this;}FastIS&operator>>(char*i){int a=get();while(~a&&(a==(char)(32)||a==(char)(13)||a==(char)(10)))a=get();while(~a&&(a!=(char)(32)&&a!=(char)(13)&&a!=(char)(10)))*i++=a,a=get();return*this;}}fin;class FastOS{static constexpr size_t P=1<<20;static constexpr double x=1e-9;char r[P],*b;const char*l;FILE*w;int o;public:FastOS(FILE*Y=stdout):b(r),l(r+P),w(Y),o(-1){setvbuf(w,nullptr,_IONBF,0);}~FastOS(){flush();fclose(w);}FastOS&flush(){fwrite(r,b-r,1,w);return*this;}FastOS&put(int g){*b++=g;if(b==l)fwrite(r,P,1,w),b=r;return*this;}FastOS&setprecision(const int&t){o=t;return*this;}template<typename a,typename=typename std::enable_if<std::is_arithmetic<a>::value>::type>FastOS&operator<<(a j){if _GLIBCXX17_CONSTEXPR(std::is_same<a,char>::value)put(j);else if _GLIBCXX17_CONSTEXPR(std::is_integral<a>::value){if _GLIBCXX17_CONSTEXPR(std::is_signed<a>::value)if(j<0)put((char)(45)),j=-j;static uint8_t i[40];i[*i=1]=j%10;while(j/=10)i[++(*i)]=j%10;while(*i)put(i[(*i)--]|(char)(48));}else if _GLIBCXX17_CONSTEXPR(std::is_floating_point<a>::value){if(j<0)put((char)(45)),j=-j;using E=typename std::conditional<std::is_same<a,float>::value,std::uint32_t,std::uint64_t>::type;E v=static_cast<E>(j);a B=j-v;*this<<v;if(o>0||(o<0&&B>x)){put((char)(46));for(int R=0;(o>0&&R<o)||o<0&&B>x;R++){int X=static_cast<int>(B*=10);put(X|(char)(48));B-=X;}}}else{throw;}return*this;}FastOS&operator<<(char*i){while(*i)put(*i++);return*this;}}fout;

struct Node {
    LL k, b;
    inline bool operator< (const Node &rhs) const {
        return k < rhs.k;
    }
} a[N];
std::vector<LL> ans;

namespace WBLT {
enum {L, R};
struct Node {
    int ch[2], w;
    LL val, add;
    inline int& operator[](bool c) { return ch[c]; }
    Node& operator()(bool);
    inline void pull(LL x) { val += x, add += x; }
    inline void pushup() {
        if (w == 1) return;
        w = (*this)(L).w + (*this)(R).w;
        val = (*this)(R).val;
    }
    inline void pushdown() {
        if (w == 1 || !add) return;
        (*this)(L).pull(add);
        (*this)(R).pull(add);
        add = 0;
    }
    inline Node() : ch{}, w(), val(), add() {}
    inline Node(LL v) : ch{}, w(1), val(v), add() {}
    inline Node(int l, int r) : ch{l, r}, add() { pushup(); }
};
std::array<Node, N << 2> tr;
namespace Data {
int bin[N << 2];
int bintot = 0, tot = 0;
template<class... Args> inline int alloc(Args... args) {
    int u = bintot ? bin[bintot--] : ++tot;
    tr[u] = Node(args...); return u;
}
inline void recycle(int u) { bin[++bintot] = u; }
} using Data::alloc; using Data::recycle;
inline bool needBalance(int wl, int wr) { return wr * 3 < wl; }
inline std::pair<int,int> cut(int u) {
    recycle(u); return {tr[u][L], tr[u][R]};
}
inline int merge(int l, int r) {
    if (!l || !r) return l | r;
    if (needBalance(tr[l].w, tr[r].w)) {
        auto [ll, lr] = cut(l);
        if (needBalance(tr[r].w + tr[lr].w, tr[ll].w)) {
            auto [lrl, lrr] = cut(lr);
            return merge(merge(ll, lrl), merge(lrr, r));
        }
        return merge(ll, merge(lr, r));
    }
    if (needBalance(tr[r].w, tr[l].w)) {
        auto [rl, rr] = cut(r);
        if (needBalance(tr[rl].w + tr[l].w, tr[rr].w)) {
            auto [rll, rlr] = cut(rl);
            return merge(merge(l, rll), merge(rlr, rr));
        }
        return merge(merge(l, rl), rr);
    }
    return merge(l, r);
}
inline void balance(int &u) {
    if (needBalance(tr[u](L).w, tr[u](R).w) || needBalance(tr[u](R).w, tr[u](L).w))
        u = merge(tr[u][L], tr[u][R]);
}
namespace Utils {
int root;
inline bool check(const ::Node &a, int u) {
    return tr[u].val <= (tr[u].w - 1ll) * a.k + a.b;
}
inline std::pair<int,int> splVal(::Node a, int u = root) {
    if (!u || tr[u].w == 1) return {u, 0};
    auto [l, r] = cut(u);
    if (check(a, l)) {
        auto [ls, rs] = splVal(a, l);
        return {merge(l, ls), rs};
    }
}
}
} // namespace WBLT
int main() {
    int n;
    fin >> n;
    for (int i = 1; i <= n; i++)
        fin >> a[i].k >> a[i].b;
    std::sort(a+1, a+1 + n);
/*
  f[i][j] = min(f[i-1][j], f[i-1][j-1] + (j-1) * a[i].k + a[i].b);
  g[i][j] = f[i][j] - f[i][j-1]
  每次一定是在已知最优集合中加一个当前天最优的，所以一定存在一个单调性
  g[i][j] = (j < k) g[i-1][j]
            (j = k) (j-1) * a[i].k + a[i].b
            (j > k) g[i-1][j-1] + a[i].k
  k 就是最小的 j 满足 g[i-1][j] <= (k-1) * a[i].k + a[i].b
*/
    for (int i = 1; i <= n; i++) { // 当前一轮：从 g[i-1][1..j-1] 转移到 g[i][1..j]
        int rk = search(a[i]);
        // printf("%d top#%d:%lld\n", rk, Splay::root, Splay::tr[Splay::root].val);
        insert(rk * a[i].k + a[i].b);
        addright(a[i].k);
        // printall(); puts("=======");
    }
    ans.reserve(n);
    getall();
    for (int i = 1; i < n; i++) ans[i] += ans[i-1];
    for (LL i : ans) fout << i << '\n';
    return 0;
}