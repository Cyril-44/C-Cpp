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

class FastIS{static constexpr size_t P=1<<20;char t[P];char*y,*I;FILE*a;public:FastIS(FILE*G=stdin):y(nullptr),I(nullptr),a(G){}~FastIS(){fclose(a);}__attribute__((always_inline)) int b(){return(y==I)&&(I=(y=t)+fread(t,1,P,a),y==I)?EOF:*y++;}template<typename F>typename std::enable_if<std::is_unsigned<F>::value&&std::is_integral<F>::value,FastIS&>::type inline operator>>(F&K){int w=b();while(~w&&(w<'0'||w>'9'))w=b();for(K=0;w>='0'&&w<='9';w=b())K=(K<<3)+(K<<1)+(w^'0');return*this;}template<typename F>typename std::enable_if<std::is_signed<F>::value&&std::is_integral<F>::value&&!std::is_same<F,bool>::value&&!std::is_same<F,char>::value,FastIS&>::type inline operator>>(F&K){int w=b();bool d=false;while(~w&&(w<'0'||w>'9')&&(w^'-'))w=b();if(w=='-')w=b(),d=true;for(K=0;w>='0'&&w<='9';w=b())K=(K<<3)+(K<<1)+(w^'0');K=d?-K:K;return*this;}template<typename F>typename std::enable_if<std::is_floating_point<F>::value,FastIS&>::type inline operator>>(F&K){int w=b();bool d=false;while(~w&&(w<'0'||w>'9')&&(w^'-'))w=b();if(w=='-')w=b(),d=true;using u=typename std::conditional<std::is_same<F,float>::value,std::uint32_t,std::uint64_t>::type;u Z;for(Z=0;w>='0'&&w<='9';w=b())Z=(Z<<3)+(Z<<1)+(w^'0');K=Z;if(w=='.'){u D=1;for(w=b();w>='0'&&w<='9';w=b())K+=(w^'0')*(1./(D*=10));}if(d)K=-K;return*this;}inline FastIS&operator>>(bool&F){int w=b();while(~w&&w!='0'&&w!='1')w=b();F=static_cast<bool>(w&1);return*this;}inline FastIS&operator>>(char&O){int w=b();while(w==' '||w=='\r'||w=='\n')w=b();O=w;return*this;}inline FastIS&operator>>(char*O){int w=b();while(~w&&(w==' '||w=='\r'||w=='\n'))w=b();while(~w&&(w!=' '&&w!='\r'&&w!='\n'))*O++=w,w=b();return*this;}}fin;class FastOS{static constexpr size_t P=1<<20;static constexpr double X=1e-9;char t[P],*A;const char*k;FILE*B;int e;public:FastOS(FILE*G=stdout):A(t),k(t+P),B(G),e(-1){setvbuf(B,nullptr,_IONBF,0);}~FastOS(){E();fclose(B);}FastOS&E(){fwrite(t,A-t,1,B);return*this;}FastOS&S(int U){*A++=U;if(A==k)fwrite(t,P,1,B),A=t;return*this;}FastOS&L(const int&V){e=V;return*this;}template<typename F>typename std::enable_if<std::is_unsigned<F>::value&&std::is_integral<F>::value,FastOS&>::type inline operator<<(F K){static int O[32];O[*O=1]=K%10;while(K/=10)O[++(*O)]=K%10;while(*O)S(O[(*O)--]|'0');return*this;}template<typename F>typename std::enable_if<std::is_signed<F>::value&&std::is_integral<F>::value&&!std::is_same<F,char>::value,FastOS&>::type inline operator<<(const F&K){if(K<0)S('-')<<(typename std::make_unsigned<F>::type)(-K);else*this<<(typename std::make_unsigned<F>::type)(K);}template<typename F>typename std::enable_if<std::is_floating_point<F>::value,FastOS&>::type inline operator<<(F K){if(K<0)S('-'),K=-K;using u=typename std::conditional<std::is_same<F,float>::value,std::uint32_t,std::uint64_t>::type;u Q=static_cast<u>(K);F q=K-Q;*this<<Q;if(e>0||e<0&&q>X){S('.');for(int a=0;e>0&&a<e||e<0&&q>X;a++){int M=static_cast<int>(q*=10);S(M|'0');q-=M;}}return*this;}FastOS&operator<<(const char&U){S(U);return*this;}}fout;

// #define MULTI_TEST_CASES

constexpr int N = 1000005;
char s[N];
int border[N];
inline void solveSingleTestCase() {
    fin >> s;
    int n = strlen(s);
    memset(border, 0, sizeof(int) * n);
    border[0] = 0;
    Forv (i, 1, n-1, j) {
        j = border[i-1];
        while (j && s[i] != s[j]) j = border[j - 1];
        if (s[i] != s[j]) border[i] = 0;
        else border[i] = j + 1; 
    }
    if (n % (n - border[n-1]) == 0) cout << n / (n - border[n-1]) << '\n';
    else cout << "1\n";
}
int main() {
    int testCases = 1;
#ifdef MULTI_TEST_CASES
    fin >> testCases;
#endif
    while (testCases--) solveSingleTestCase();
    return 0;
}