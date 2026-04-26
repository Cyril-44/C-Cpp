#include <bits/stdc++.h>
#define LUOGU
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
#define YES return cout << "Yes\n", void()
#define NO return cout << "No\n", void()
#define YESNO(j) cout << ((j) ? "Yes\n" : "No\n")
using namespace std;using pii_t=pair<int,int>;using pll_t=pair<int64_t,int64_t>;using veci_t=vector<int>;using vecl_t=vector<int64_t>;Inline int Popcnt(int x){return __builtin_popcount((unsigned)x);}Inline int Popcnt(unsigned x){return __builtin_popcount(x);}Inline int Popcnt(int64_t x){return __builtin_popcountll((uint64_t)x);}Inline int Popcnt(uint64_t x){return __builtin_popcountll(x);}Inline int Log2(int x){return 31-__builtin_clz((unsigned)x|1);}Inline int Log2(unsigned x){return 31-__builtin_clz(x|1);}Inline int Log2(int64_t x){return 63-__builtin_clzll((uint64_t)x|1);}Inline int Log2(uint64_t x){return 63-__builtin_clzll(x|1);}

// #define MULTI_TEST_CASES

constexpr int N = 5005; // To be modified
veci_t g[N];
template<typename b>b modInv(const b&c,const b&d){assert(d!=0);b e=0,f=1,g=c,h=d,i;while(g!=0){i=h/g;std::swap(g,h-=i*g);std::swap(e-=i*f,f);}assert(h==1);return e;}template<class k,typename k::value_type l=0>requires requires(typename k::value_type m,typename k::value_type n){{m+n}->std::same_as<typename k::value_type>;{m-n}->std::same_as<typename k::value_type>;{m*n}->std::same_as<typename k::value_type>;{m%n}->std::same_as<typename k::value_type>;}class ModInt{using o=k::value_type;inline static constexpr o p(){return k::value;}o q;template<typename s>inline o r(s t){if constexpr(std::is_unsigned_v<s>)return static_cast<o>(t<p()?t:t%p());else{o u=static_cast<o>(-p()<t&&t<p()?t:t%p());return(u<0?u+p():u);}}public:constexpr inline ModInt():q(l){}template<typename v>inline ModInt(const v&w){q=r(w);}template<typename x>inline explicit operator x()const{return static_cast<x>(q);}inline o operator()()const{return q;}inline ModInt&operator+=(const ModInt&y){if((q+=y.q)>=p())q-=p();return*this;}inline ModInt&operator-=(const ModInt&z){if((q-=z.q)<0)q+=p();return*this;}inline ModInt&operator*=(const ModInt&A){if constexpr(std::is_same_v<o,int>)q=r((uint64_t)q*A.q);else if constexpr(std::is_same_v<o,int64_t>)q=r((unsigned __int128)q*A.q);else q=r(q*A.q);return*this;}inline ModInt&operator/=(const ModInt&B){return*this*=ModInt(modInv(B.q,p()));}template<std::integral C>inline ModInt&operator^=(C D){if(D<0)return*this=ModInt(modInv(static_cast<o>(1),(*this^(-D))()));ModInt E=*this;for(*this=static_cast<o>(1);D;D>>=1){if(D&1)*this*=E;E*=E;}return*this;}inline ModInt operator-()const{return ModInt(-q);}inline ModInt&operator++(){return*this+=1;}inline ModInt&operator--(){return*this-=1;}inline ModInt operator++(int){ModInt F=*this;++*this;return F;}inline ModInt operator--(int){ModInt G=*this;--*this;return G;}inline ModInt operator+(const ModInt&H)const{return ModInt(*this)+=H;}inline ModInt operator-(const ModInt&I)const{return ModInt(*this)-=I;}inline ModInt operator*(const ModInt&J)const{return ModInt(*this)*=J;}inline ModInt operator/(const ModInt&K)const{return ModInt(*this)/=K;}inline ModInt operator^(const ModInt&L)const{return ModInt(*this)^=L;}inline bool operator==(const ModInt&M)const{return q==M.q;}inline bool operator!=(const ModInt&N)const{return q!=N.q;}inline bool operator<=(const ModInt&O)const{return q<=O.q;}inline bool operator>=(const ModInt&P)const{return q>=P.q;}inline bool operator<(const ModInt&Q)const{return q<Q.q;}inline bool operator>(const ModInt&R)const{return q>R.q;}inline bool operator!()const{return q;}template<typename S>inline friend ModInt operator+(const S&T,const ModInt&U){return ModInt(T)+U;}template<typename V>inline friend ModInt operator-(const V&W,const ModInt&X){return ModInt(W)-X;}template<typename Y>inline friend ModInt operator*(const Y&Z,const ModInt&_){return ModInt(Z)*_;}template<typename ab>inline friend ModInt operator/(const ab&bb,const ModInt&cb){return ModInt(bb)/cb;}template<typename db>inline friend ModInt operator==(const db&eb,const ModInt&fb){return ModInt(eb)==fb;}template<typename gb>inline friend ModInt operator!=(const gb&hb,const ModInt&ib){return ModInt(hb)!=ib;}template<typename jb>inline friend ModInt operator<=(const jb&kb,const ModInt&lb){return ModInt(kb)<=lb;}template<typename mb>inline friend ModInt operator>=(const mb&nb,const ModInt&ob){return ModInt(nb)>=ob;}template<typename pb>inline friend ModInt operator<(const pb&qb,const ModInt&rb){return ModInt(qb)<rb;}template<typename sb>inline friend ModInt operator>(const sb&tb,const ModInt&ub){return ModInt(tb)>ub;}template<typename vb>inline friend vb&operator>>(vb&wb,ModInt&xb){wb>>xb.q;xb.q=xb.r(xb.q);return wb;}template<typename yb>friend inline yb&operator<<(yb&zb,const ModInt&Ab){return zb<<Ab.q;}};
constexpr auto MOD = 998244353;
using Mint = ModInt<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
// struct Dynamic_ModInt { using value_type = int; static value_type value; };
// Dynamic_ModInt::value_type &Mod = Dynamic_ModInt::value;
// using Mint = ModInt<Dynamic_ModInt>;
struct Fact{Fact(const int&T):E(T+1,Mint(1)),H(T+1),v(T){E[0]=1;for(int J=1;J<=T;J++)E[J]=E[J-1]*J;H[T]=Mint(1)/E[T];for(int J=T;J>=1;J--)H[J-1]=H[J]*J;}Mint C(const int&T,const int&I)const{if(T<0||I<0||T<I)return 0;if(T>v)throw std::out_of_range("\u0045\u0078\u0070\u0065\u0063\u0074\u0065\u0064\u0020\u006E\u0020\u003C\u0020"+std::to_string(v)+"\u002C\u0020\u0062\u0075\u0074\u0020\u0066\u006F\u0075\u006E\u0064\u0020\u006E\u0020\u003D\u0020"+std::to_string(T)+"\u002E");return E[T]*H[I]*H[T-I];}Mint A(const int&T,const int&I)const{if(T<0||I<0||T<I)return 0;if(T>v)throw std::out_of_range("\u0045\u0078\u0070\u0065\u0063\u0074\u0065\u0064\u0020\u006E\u0020\u003C\u0020"+std::to_string(v)+"\u002C\u0020\u0062\u0075\u0074\u0020\u0066\u006F\u0075\u006E\u0064\u0020\u006E\u0020\u003D\u0020"+std::to_string(T)+"\u002E");return E[T]*H[T-I];}private:std::vector<Mint>E,H;const int v;};

Mint f[N], prod[N], ways[N], iways[N];
int dep[N];
void dfs(int u, int fa) {
    if (u == 1) ways[u] = g[u].size();
    else ways[u] = dep[u] + g[u].size() - 1;
    iways[u] = 1 / ways[u];
    prod[u] = ways[u];
    for (int v : g[u]) if (v != fa) {
        dep[v] = dep[u] + 1;
        dfs(v, u);
        f[u] += (f[v] + iways[v]) * iways[u];
        prod[u] *= prod[v];
    }
}

inline void solveSingleTestCase() {
    int n;
    cin >> n;
    Repv (n-1, u, v) {
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs(1, 0);
    Mint ans;
    For (i, 1, n) ans += f[i] * prod[1];
    cout << ans << '\n';
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int testCases = 1;
#ifdef MULTI_TEST_CASES
    cin >> testCases;
#endif
    while (testCases--) solveSingleTestCase();
    return 0;
}