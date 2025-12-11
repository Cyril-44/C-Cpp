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

#define MULTI_TEST_CASES
template<typename E>E modInv(const E&P,const E&M){assert(P!=0);E w=0,N=1,U=P,Q=M,A;while(U!=0){A=Q/U;std::swap(U,Q-=A*U);std::swap(w-=A*N,N);}assert(Q==1);return w;}template<typename Mod>class MB{using F=typename std::decay<decltype(Mod::value)>::type;F value;constexpr static F o(){return Mod::value;}template<typename E>E L(E P){F H=static_cast<F>(-o()<P&&P<o()?P:P%o());return(H<0?H+=o():H);}public:constexpr MB():value(){}template<typename E>MB(const E&P){value=L(P);}template<typename E>explicit operator E()const{return static_cast<E>(value);}const F&operator()()const{return value;}MB&operator+=(const MB&P){if((value+=P.value)>=o())value-=o();return*this;}MB&operator-=(const MB&P){if((value-=P.value)<0)value+=o();return*this;}MB&operator*=(const MB&P){if constexpr(std::is_same<F,int>::value)value=L(static_cast<int64_t>(value)*P.value);else value=L(value*P.value);return*this;}MB&operator/=(const MB&P){return*this*=MB(modInv(P.value,o()));}template<typename E>typename std::enable_if<std::is_integral<E>::value,MB>::type&operator^=(E P){if(P<0)return*this=MB(modInv(static_cast<F>(1),(*this^(-P))()));MB e=*this;for(*this=static_cast<F>(1);P;P>>=1){if(P&1)*this*=e;e*=e;}return*this;}MB operator-()const{return MB(-value);}MB&operator++(){return*this+=1;}MB&operator--(){return*this-=1;}MB operator++(int){MB e=*this;++*this;return e;}MB operator--(int){MB e=*this;--*this;return e;}MB operator+(const MB&P)const{return MB(*this)+=P;}MB operator-(const MB&P)const{return MB(*this)-=P;}MB operator*(const MB&P)const{return MB(*this)*=P;}MB operator/(const MB&P)const{return MB(*this)/=P;}bool operator==(const MB&P)const{return value==P.value;}bool operator!=(const MB&P)const{return value!=P.value;}bool operator<=(const MB&P)const{return value<=P.value;}bool operator>=(const MB&P)const{return value>=P.value;}bool operator<(const MB&P)const{return value<P.value;}bool operator>(const MB&P)const{return value>P.value;}bool operator!()const{return value;}template<typename q,typename E>friend q&operator>>(q&,MB<E>&);template<typename I,typename E>friend I&operator<<(I&,const MB<E>&);template<typename N>friend MB operator+(const MB&J,const N&P){return J+MB(P);}template<typename N>friend MB operator+(const N&J,const MB&P){return MB(J)+P;}template<typename N>friend MB operator-(const MB&J,const N&P){return J-MB(P);}template<typename N>friend MB operator-(const N&J,const MB&P){return MB(J)-P;}template<typename N>friend MB operator*(const MB&J,const N&P){return J*MB(P);}template<typename N>friend MB operator*(const N&J,const MB&P){return MB(J)*P;}template<typename N>friend MB operator/(const MB&J,const N&P){return J/MB(P);}template<typename N>friend MB operator/(const N&J,const MB&P){return MB(J)/P;}template<typename N>friend typename std::enable_if<std::is_integral<N>::value,MB>::type operator^(MB J,const N&P){return J^=P;}template<typename N>friend MB operator+=(MB&J,const N&P){return J+=MB(P);}template<typename N>friend MB operator-=(MB&J,const N&P){return J-=MB(P);}template<typename N>friend MB operator*=(MB&J,const N&P){return J*=MB(P);}template<typename N>friend MB operator/=(MB&J,const N&P){return J/=MB(P);}template<typename N>friend MB operator==(const MB&J,const N&P){return J==MB(P);}template<typename N>friend MB operator==(const N&J,const MB&P){return MB(J)==P;}template<typename N>friend MB operator!=(const MB&J,const N&P){return J!=MB(P);}template<typename N>friend MB operator!=(const N&J,const MB&P){return MB(J)!=P;}template<typename N>friend MB operator<=(const MB&J,const N&P){return J<=MB(P);}template<typename N>friend MB operator<=(const N&J,const MB&P){return MB(J)<=P;}template<typename N>friend MB operator>=(const MB&J,const N&P){return J>=MB(P);}template<typename N>friend MB operator>=(const N&J,const MB&P){return MB(J)>=P;}template<typename N>friend MB operator<(const MB&J,const N&P){return J<MB(P);}template<typename N>friend MB operator<(const N&J,const MB&P){return MB(J)<P;}template<typename N>friend MB operator>(const MB&J,const N&P){return J>MB(P);}template<typename N>friend MB operator>(const N&J,const MB&P){return MB(J)>P;}};template<typename q,typename E>q&operator>>(q&g,MB<E>&J){typename MB<E>::F K;g>>K;J.value=J.L(K);return g;}template<typename I,typename E>I&operator<<(I&C,const MB<E>&P){return C<<P.value;}
using ModType = int;
constexpr ModType MOD = 998244353;
using Mint = MB<std::integral_constant<ModType, MOD>>;
// struct b { static ModType D; };
// ModType &Mod = b::D;
// using Modular = MB<b>;
struct Fact{Fact(const int&E):d(E+1,Mint(1)),I(E+1),Z(E){d[0]=1;for(int Y=1;Y<=E;Y++)d[Y]=d[Y-1]*Y;I[E]=Mint(1)/d[E];for(int Y=E;Y>=1;Y--)I[Y-1]=I[Y]*Y;}Mint C(const int&E,const int&F)const{if(E<0||F<0||E<F)return 0;if(E>Z)throw std::out_of_range("\u0045\u0078\u0070\u0065\u0063\u0074\u0065\u0064\u0020\u006E\u0020\u003C\u0020"+std::to_string(Z)+"\u002C\u0020\u0062\u0075\u0074\u0020\u0066\u006F\u0075\u006E\u0064\u0020\u006E\u0020\u003D\u0020"+std::to_string(E)+"\u002E");return d[E]*I[F]*I[E-F];}Mint A(const int&E,const int&F)const{if(E<0||F<0||E<F)return 0;if(E>Z)throw std::out_of_range("\u0045\u0078\u0070\u0065\u0063\u0074\u0065\u0064\u0020\u006E\u0020\u003C\u0020"+std::to_string(Z)+"\u002C\u0020\u0062\u0075\u0074\u0020\u0066\u006F\u0075\u006E\u0064\u0020\u006E\u0020\u003D\u0020"+std::to_string(E)+"\u002E");return d[E]*I[E-F];}private:std::vector<Mint>d,I;const int Z;};

constexpr int N = 200005;
constexpr int64_t MX = 1e18;
vector<int> g[N], ng[N];
int64_t a[N];
map<pair<int, pair<int64_t, int64_t>>, Mint> mp;
int64_t fib[205];
Mint dfs(int u, int dep) {
    if (dep > fib[0]) return 0;
    auto key = make_pair(u, make_pair(fib[dep-1], fib[dep]));
    if (mp.count(key))
        return mp[key];
    Mint ans = 1;
    for (int v : g[u]) {
        if (a[v] == fib[dep])
            ans += dfs(v, dep + 1);
    }
    return ans;
}
inline void solveSingleTestCase() {
    int n, m;
    cin >> n >> m;
    For (i, 1, n) {
        cin >> a[i];
        g[i].clear();
    }
    mp.clear();
    Repv (m, u, v) {
        cin >> u >> v;
        g[u].push_back(v);
    }
    Mint ans;
    For (i, 1, n) {
        for (int j : g[i]) {
            int64_t fn = a[i], fn_1 = a[j];
            fib[1] = a[i], fib[2] = a[j];
            for (fib[0] = 3; ; ++fib[0]) {
                swap(fn, fn_1);
                fn += fn_1;
                if (fn > MX) break;
                fib[fib[0]] = fn;
            }
            --fib[0];
            ans += dfs(j, 3);
        }
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