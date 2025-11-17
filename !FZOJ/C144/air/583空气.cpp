#include <bits/stdc++.h>
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
#define YES return cout << "Yes\n", void()
#define NO return cout << "No\n", void()
#define YESNO(j) cout << ((j) ? "Yes\n" : "No\n")
using namespace std;using pii_t=pair<int,int>;using pll_t=pair<int64_t,int64_t>;using veci_t=vector<int>;using vecl_t=vector<int64_t>;Inline int Popcnt(int x){return __builtin_popcount((unsigned)x);}Inline int Popcnt(unsigned x){return __builtin_popcount(x);}Inline int Popcnt(int64_t x){return __builtin_popcountll((uint64_t)x);}Inline int Popcnt(uint64_t x){return __builtin_popcountll(x);}Inline int Log2(int x){return 31-__builtin_clz((unsigned)x|1);}Inline int Log2(unsigned x){return 31-__builtin_clz(x|1);}Inline int Log2(int64_t x){return 63-__builtin_clzll((uint64_t)x|1);}Inline int Log2(uint64_t x){return 63-__builtin_clzll(x|1);}

// #define MULTI_TEST_CASES


template<typename U>U modInv(const U&Y,const U&m){assert(Y!=0);U u=0,L=1,w=Y,W=m,B;while(w!=0){B=W/w;std::swap(w,W-=B*w);std::swap(u-=B*L,L);}assert(W==1);return u;}template<typename Mod>class MB{using o=typename std::decay<decltype(Mod::value)>::type;o value;constexpr static o Y(){return Mod::value;}template<typename U>U c(U Y){o r=static_cast<o>(-Y()<Y&&Y<Y()?Y:Y%Y());return(r<0?r+=Y():r);}public:constexpr MB():value(){}template<typename U>MB(const U&J){value=c(J);}template<typename U>explicit operator U()const{return static_cast<U>(value);}const o&operator()()const{return value;}MB&operator+=(const MB&J){if((value+=J.value)>=Y())value-=Y();return*this;}MB&operator-=(const MB&J){if((value-=J.value)<0)value+=Y();return*this;}MB&operator*=(const MB&J){if constexpr(std::is_same<o,int>::value)value=c(static_cast<int64_t>(value)*J.value);else value=c(value*J.value);return*this;}MB&operator/=(const MB&J){return*this*=MB(modInv(J.value,Y()));}template<typename U>typename std::enable_if<std::is_integral<U>::value,MB>::type&operator^=(U J){if(J<0)return*this=MB(modInv(static_cast<o>(1),(*this^(-J))()));MB Q=*this;for(*this=static_cast<o>(1);J;J>>=1){if(J&1)*this*=Q;Q*=Q;}return*this;}MB operator-()const{return MB(-value);}MB&operator++(){return*this+=1;}MB&operator--(){return*this-=1;}MB operator++(int){MB Q=*this;++*this;return Q;}MB operator--(int){MB Q=*this;--*this;return Q;}MB operator+(const MB&J)const{return MB(*this)+=J;}MB operator-(const MB&J)const{return MB(*this)-=J;}MB operator*(const MB&J)const{return MB(*this)*=J;}MB operator/(const MB&J)const{return MB(*this)/=J;}bool operator==(const MB&J)const{return value==J.value;}bool operator!=(const MB&J)const{return value!=J.value;}bool operator<=(const MB&J)const{return value<=J.value;}bool operator>=(const MB&J)const{return value>=J.value;}bool operator<(const MB&J)const{return value<J.value;}bool operator>(const MB&J)const{return value>J.value;}bool operator!()const{return value;}template<typename I,typename U>friend I&operator>>(I&,MB<U>&);template<typename B,typename U>friend B&operator<<(B&,const MB<U>&);template<typename O>friend MB operator+(const MB&E,const O&J){return E+MB(J);}template<typename O>friend MB operator+(const O&E,const MB&J){return MB(E)+J;}template<typename O>friend MB operator-(const MB&E,const O&J){return E-MB(J);}template<typename O>friend MB operator-(const O&E,const MB&J){return MB(E)-J;}template<typename O>friend MB operator*(const MB&E,const O&J){return E*MB(J);}template<typename O>friend MB operator*(const O&E,const MB&J){return MB(E)*J;}template<typename O>friend MB operator/(const MB&E,const O&J){return E/MB(J);}template<typename O>friend MB operator/(const O&E,const MB&J){return MB(E)/J;}template<typename O>friend typename std::enable_if<std::is_integral<O>::value,MB>::type operator^(MB E,const O&J){return E^=J;}template<typename O>friend MB operator+=(MB&E,const O&J){return E+=MB(J);}template<typename O>friend MB operator-=(MB&E,const O&J){return E-=MB(J);}template<typename O>friend MB operator*=(MB&E,const O&J){return E*=MB(J);}template<typename O>friend MB operator/=(MB&E,const O&J){return E/=MB(J);}template<typename O>friend MB operator==(const MB&E,const O&J){return E==MB(J);}template<typename O>friend MB operator==(const O&E,const MB&J){return MB(E)==J;}template<typename O>friend MB operator!=(const MB&E,const O&J){return E!=MB(J);}template<typename O>friend MB operator!=(const O&E,const MB&J){return MB(E)!=J;}template<typename O>friend MB operator<=(const MB&E,const O&J){return E<=MB(J);}template<typename O>friend MB operator<=(const O&E,const MB&J){return MB(E)<=J;}template<typename O>friend MB operator>=(const MB&E,const O&J){return E>=MB(J);}template<typename O>friend MB operator>=(const O&E,const MB&J){return MB(E)>=J;}template<typename O>friend MB operator<(const MB&E,const O&J){return E<MB(J);}template<typename O>friend MB operator<(const O&E,const MB&J){return MB(E)<J;}template<typename O>friend MB operator>(const MB&E,const O&J){return E>MB(J);}template<typename O>friend MB operator>(const O&E,const MB&J){return MB(E)>J;}};template<typename I,typename U>I&operator>>(I&M,MB<U>&E){typename MB<U>::o V;M>>V;E.value=E.c(V);return M;}template<typename B,typename U>B&operator<<(B&z,const MB<U>&J){return z<<J.value;}
using ModType = int;
constexpr ModType MOD = 1e9 + 7;
using Modular = MB<std::integral_constant<ModType, MOD>>;
// struct b { static ModType D; };
// ModType &Mod = b::D;
// using Modular = MB<b>;
struct Fact{Fact(const int&E):d(E+1,Modular(1)),I(E+1),Z(E){d[0]=1;for(int Y=1;Y<=E;Y++)d[Y]=d[Y-1]*Y;I[E]=Modular(1)/d[E];for(int Y=E;Y>=1;Y--)I[Y-1]=I[Y]*Y;}Modular C(const int&E,const int&F)const{if(E<0||F<0||E<F)return 0;if(E>Z)throw std::out_of_range("\u0045\u0078\u0070\u0065\u0063\u0074\u0065\u0064\u0020\u006E\u0020\u003C\u0020"+std::to_string(Z)+"\u002C\u0020\u0062\u0075\u0074\u0020\u0066\u006F\u0075\u006E\u0064\u0020\u006E\u0020\u003D\u0020"+std::to_string(E)+"\u002E");return d[E]*I[F]*I[E-F];}Modular A(const int&E,const int&F)const{if(E<0||F<0||E<F)return 0;if(E>Z)throw std::out_of_range("\u0045\u0078\u0070\u0065\u0063\u0074\u0065\u0064\u0020\u006E\u0020\u003C\u0020"+std::to_string(Z)+"\u002C\u0020\u0062\u0075\u0074\u0020\u0066\u006F\u0075\u006E\u0064\u0020\u006E\u0020\u003D\u0020"+std::to_string(E)+"\u002E");return d[E]*I[E-F];}private:std::vector<Modular>d,I;const int Z;};

constexpr int N = 1e6;
Fact fac(N*2);

inline void solveSingleTestCase() {
    int n, m;
    cin >> n >> m;
    Modular ans(0);
    For(i, 0, min(n-1, m)) {
        ans += Modular(n-i) * (fac.C(n+m, m+(n-i)) - fac.C(n+m, m+(n-i)+1));
    }
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