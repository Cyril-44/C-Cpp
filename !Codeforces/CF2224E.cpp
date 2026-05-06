#include <bits/stdc++.h>
// #define LUOGU
#if defined(ONLINE_JUDGE) && !defined(LUOGU)
# pragma GCC optimize(2, 3, "inline", "unroll-loops", "fast-math", "inline-small-functions", "no-stack-protector", "delete-null-pointer-checks")
# pragma GCC target("tune=native")
# pragma GCC target("arch=native")
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
class FastIS{static constexpr size_t h=1<<20;char o[h];char*t,*v;FILE*K;public:FastIS(FILE*B=stdin):t(nullptr),v(nullptr),K(B){}~FastIS(){fclose(K);}Inline int get(){return(t==v)&&(v=(t=o)+fread(o,1,h,K),t==v)?EOF:*t++;}template<typename j>typename std::enable_if<std::is_unsigned<j>::value&&std::is_integral<j>::value,FastIS&>::type inline operator>>(j&d){int a=get();while(~a&&(a<'0'||a>'9'))a=get();for(d=0;a>='0'&&a<='9';a=get())d=(d<<3)+(d<<1)+(a^'0');return*this;}template<typename j>typename std::enable_if<std::is_signed<j>::value&&std::is_integral<j>::value&&!std::is_same<j,bool>::value&&!std::is_same<j,char>::value,FastIS&>::type inline operator>>(j&d){int a=get();bool L=false;while(~a&&(a<'0'||a>'9')&&(a^'-'))a=get();if(a=='-')a=get(),L=true;for(d=0;a>='0'&&a<='9';a=get())d=(d<<3)+(d<<1)+(a^'0');d=L?-d:d;return*this;}template<typename j>typename std::enable_if<std::is_floating_point<j>::value,FastIS&>::type inline operator>>(j&d){int a=get();bool L=false;while(~a&&(a<'0'||a>'9')&&(a^'-'))a=get();if(a=='-')a=get(),L=true;using G=typename std::conditional<std::is_same<j,float>::value,std::uint32_t,std::uint64_t>::type;G I;for(I=0;a>='0'&&a<='9';a=get())I=(I<<3)+(I<<1)+(a^'0');d=I;if(a=='.'){G w=1;for(a=get();a>='0'&&a<='9';a=get())d+=(a^'0')*(1./(w*=10));}if(L)d=-d;return*this;}inline FastIS&operator>>(bool&U){int a=get();while(~a&&a!='0'&&a!='1')a=get();U=static_cast<bool>(a&1);return*this;}inline FastIS&operator>>(char&n){int a=get();while(a==' '||a=='\r'||a=='\n')a=get();n=a;return*this;}inline FastIS&operator>>(char*n){int a=get();while(~a&&(a==' '||a=='\r'||a=='\n'))a=get();while(~a&&(a!=' '&&a!='\r'&&a!='\n'))*n++=a,a=get();return*this;}}fin;class FastOS{static constexpr size_t h=1<<20;static constexpr double R=1e-9;char o[h],*O;const char*y;FILE*q;int l;public:FastOS(FILE*B=stdout):O(o),y(o+h),q(B),l(-1){setvbuf(q,nullptr,_IONBF,0);}~FastOS(){flush();fclose(q);}FastOS&flush(){fwrite(o,O-o,1,q);return*this;}FastOS&put(int H){*O++=H;if(O==y)fwrite(o,h,1,q),O=o;return*this;}FastOS&setprecision(const int&S){l=S;return*this;}template<typename j>typename std::enable_if<std::is_unsigned<j>::value&&std::is_integral<j>::value,FastOS&>::type inline operator<<(j d){static int n[32];n[*n=1]=d%10;while(d/=10)n[++(*n)]=d%10;while(*n)put(n[(*n)--]|'0');return*this;}template<typename j>typename std::enable_if<std::is_signed<j>::value&&std::is_integral<j>::value&&!std::is_same<j,char>::value,FastOS&>::type inline operator<<(const j&d){if(d<0)put('-')<<(typename std::make_unsigned<j>::type)(-d);else*this<<(typename std::make_unsigned<j>::type)(d);return *this;}template<typename j>typename std::enable_if<std::is_floating_point<j>::value,FastOS&>::type inline operator<<(j d){if(d<0)put('-'),d=-d;using G=typename std::conditional<std::is_same<j,float>::value,std::uint32_t,std::uint64_t>::type;G z=static_cast<G>(d);j O=d-z;*this<<z;if(l>0||l<0&&O>R){put('.');for(int J=0;l>0&&J<l||l<0&&O>R;J++){int N=static_cast<int>(O*=10);put(N|'0');O-=N;}}return*this;}FastOS&operator<<(const char&H){put(H);return*this;}}fout;
constexpr int N = 5000; // TO BE MODIFIED!!!
vector<int> g[N];
int fa[N], l[N], d[N];
uint64_t m[N];
inline void solveSingleTestCase() {
    int n, q;
    cin >> n >> q;
    For (i, 1, n) g[i].clear();
    memset(d+1, 0, sizeof(*d) * n);
    For (i, 2, n) {
        cin >> fa[i];
        ++d[fa[i]];
        g[fa[i]].push_back(i);
    }
    For (i, 1, n) cin >> m[i];

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