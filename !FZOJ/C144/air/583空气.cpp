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

template<typename b>b modInv(const b&v,const b&p){assert(v!=0);b J=0,q=1,a=v,I=p,D;while(a!=0){D=I/a;std::swap(a,I-=D*a);std::swap(J-=D*q,q);}assert(I==1);return J;}template<typename Md>class MB{using S=typename std::enable_if<std::is_signed<typename std::decay<decltype(Md::value)>::type>::value,typename std::decay<decltype(Md::value)>::type>::type;S value;constexpr static S D(){return Md::value;}template<typename b>b L(b v){if _GLIBCXX17_CONSTEXPR(std::is_unsigned<b>::value)return static_cast<S>(v<D()?v:v%D());else{S h=static_cast<S>(-D()<v&&v<D()?v:v%D());return(h<0?h+D():h);}}public:constexpr MB():value(){}template<typename b>MB(const b&z){value=L(z);}template<typename b>explicit operator b()const{return static_cast<b>(value);}const S&operator()()const{return value;}MB&operator+=(const MB&z){if((value+=z.value)>=D())value-=D();return*this;}MB&operator-=(const MB&z){if((value-=z.value)<0)value+=D();return*this;}MB&operator*=(const MB&z){if _GLIBCXX17_CONSTEXPR(std::is_same<S,int>::value)value=L((uint64_t)value*z.value);else if _GLIBCXX17_CONSTEXPR(std::is_same<S,int64_t>::value)value=L((unsigned __int128)value*z.value);else value=L(value*z.value);return*this;}MB&operator/=(const MB&z){return*this*=MB(modInv(z.value,D()));}template<typename b>typename std::enable_if<std::is_integral<b>::value,MB>::type&operator^=(b z){if(z<0)return*this=MB(modInv(static_cast<S>(1),(*this^(-z))()));MB j=*this;for(*this=static_cast<S>(1);z;z>>=1){if(z&1)*this*=j;j*=j;}return*this;}MB operator-()const{return MB(-value);}MB&operator++(){return*this+=1;}MB&operator--(){return*this-=1;}MB operator++(int){MB j=*this;++*this;return j;}MB operator--(int){MB j=*this;--*this;return j;}MB operator+(const MB&z)const{return MB(*this)+=z;}MB operator-(const MB&z)const{return MB(*this)-=z;}MB operator*(const MB&z)const{return MB(*this)*=z;}MB operator/(const MB&z)const{return MB(*this)/=z;}bool operator==(const MB&z)const{return value==z.value;}bool operator!=(const MB&z)const{return value!=z.value;}bool operator<=(const MB&z)const{return value<=z.value;}bool operator>=(const MB&z)const{return value>=z.value;}bool operator<(const MB&z)const{return value<z.value;}bool operator>(const MB&z)const{return value>z.value;}bool operator!()const{return value;}template<typename R,typename b>friend R&operator>>(R&,MB<b>&);template<typename g,typename b>friend g&operator<<(g&,const MB<b>&);template<typename G>friend MB operator+(const MB&T,const G&z){return T+MB(z);}template<typename G>friend MB operator+(const G&T,const MB&z){return MB(T)+z;}template<typename G>friend MB operator-(const MB&T,const G&z){return T-MB(z);}template<typename G>friend MB operator-(const G&T,const MB&z){return MB(T)-z;}template<typename G>friend MB operator*(const MB&T,const G&z){return T*MB(z);}template<typename G>friend MB operator*(const G&T,const MB&z){return MB(T)*z;}template<typename G>friend MB operator/(const MB&T,const G&z){return T/MB(z);}template<typename G>friend MB operator/(const G&T,const MB&z){return MB(T)/z;}template<typename G>friend typename std::enable_if<std::is_integral<G>::value,MB>::type operator^(MB T,const G&z){return T^=z;}template<typename G>friend MB operator+=(MB&T,const G&z){return T+=MB(z);}template<typename G>friend MB operator-=(MB&T,const G&z){return T-=MB(z);}template<typename G>friend MB operator*=(MB&T,const G&z){return T*=MB(z);}template<typename G>friend MB operator/=(MB&T,const G&z){return T/=MB(z);}template<typename G>friend MB operator==(const MB&T,const G&z){return T==MB(z);}template<typename G>friend MB operator==(const G&T,const MB&z){return MB(T)==z;}template<typename G>friend MB operator!=(const MB&T,const G&z){return T!=MB(z);}template<typename G>friend MB operator!=(const G&T,const MB&z){return MB(T)!=z;}template<typename G>friend MB operator<=(const MB&T,const G&z){return T<=MB(z);}template<typename G>friend MB operator<=(const G&T,const MB&z){return MB(T)<=z;}template<typename G>friend MB operator>=(const MB&T,const G&z){return T>=MB(z);}template<typename G>friend MB operator>=(const G&T,const MB&z){return MB(T)>=z;}template<typename G>friend MB operator<(const MB&T,const G&z){return T<MB(z);}template<typename G>friend MB operator<(const G&T,const MB&z){return MB(T)<z;}template<typename G>friend MB operator>(const MB&T,const G&z){return T>MB(z);}template<typename G>friend MB operator>(const G&T,const MB&z){return MB(T)>z;}};template<typename R,typename b>R&operator>>(R&w,MB<b>&T){typename MB<b>::S C;w>>C;T.value=T.L(C);return w;}template<typename g,typename b>g&operator<<(g&f,const MB<b>&z){return f<<z.value;}
constexpr auto MOD = (int)1e9 + 7;
using Mint = MB<std::integral_constant<decltype(MOD), MOD>>;
// struct A { using type = int; static type value; };
// d &Mod = A::value;
// using Modular = F<A>;
struct Fact{Fact(const int&T):E(T+1,Mint(1)),H(T+1),v(T){E[0]=1;for(int J=1;J<=T;J++)E[J]=E[J-1]*J;H[T]=Mint(1)/E[T];for(int J=T;J>=1;J--)H[J-1]=H[J]*J;}Mint C(const int&T,const int&I)const{if(T<0||I<0||T<I)return 0;if(T>v)throw std::out_of_range("\u0045\u0078\u0070\u0065\u0063\u0074\u0065\u0064\u0020\u006E\u0020\u003C\u0020"+std::to_string(v)+"\u002C\u0020\u0062\u0075\u0074\u0020\u0066\u006F\u0075\u006E\u0064\u0020\u006E\u0020\u003D\u0020"+std::to_string(T)+"\u002E");return E[T]*H[I]*H[T-I];}Mint A(const int&T,const int&I)const{if(T<0||I<0||T<I)return 0;if(T>v)throw std::out_of_range("\u0045\u0078\u0070\u0065\u0063\u0074\u0065\u0064\u0020\u006E\u0020\u003C\u0020"+std::to_string(v)+"\u002C\u0020\u0062\u0075\u0074\u0020\u0066\u006F\u0075\u006E\u0064\u0020\u006E\u0020\u003D\u0020"+std::to_string(T)+"\u002E");return E[T]*H[T-I];}private:std::vector<Mint>E,H;const int v;};
Fact fac(2e6);

inline void solveSingleTestCase() {
    int n, m;
    cin >> n >> m;
    Mint ans = 0;
/*
向右上走
$(0,0)$ 到 $(n,m)$
一定触碰到 $y=x+k$
方案数：   
$\dbinom{n+m}{n+k} - \dbinom{n+m}{n+k+1}$
*/
    For (k, max(n - m, 1), n) { // +1 上， -1 右； 算碰到了且不超过 y=x+k
        ans += k * (fac.C(n+m, n-k) - fac.C(n+m, n-k-1));
    }
    cout << ans << '\n';
}
int main() {
    freopen("air.in", "r", stdin);
    freopen("air.out", "w", stdout);
    cin.tie(nullptr) -> sync_with_stdio(false);
    int testCases = 1;
#ifdef MULTI_TEST_CASES
    cin >> testCases;
#endif
    while (testCases--) solveSingleTestCase();
    return 0;
}