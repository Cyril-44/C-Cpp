#include <bits/stdc++.h>
// #define LUOGU
#if defined(ONLINE_JUDGE) && !defined(LUOGU)
#pragma GCC optimize(2, 3, "inline", "unroll-loops", "fast-math",              \
                     "inline-small-functions", "no-stack-protector",           \
                     "delete-null-pointer-checks")
#pragma GCC target("tune=native")
#endif
#define Inline __attribute__((always_inline)) inline
#define For(i, s, t) for (int i = (s); i <= (t); ++i)
#define Forv(i, s, t, ...) for (int i = (s), __VA_ARGS__; i <= (t); ++i)
#define roF(i, t, s) for (int i = (t); i >= (s); --i)
#define roFv(i, t, s, ...) for (int i = (t), __VA_ARGS__; i >= (s); --i)
#define Rep(c) for (int tempFor_count = c; tempFor_count; --tempFor_count)
#define Repv(c, ...)                                                           \
    for (int tempFor_count = c, __VA_ARGS__; tempFor_count; --tempFor_count)
using namespace std;
using pii_t = pair<int, int>;
using pll_t = pair<int64_t, int64_t>;
using veci_t = vector<int>;
using vecl_t = vector<int64_t>;
Inline int Popcnt(int x) { return __builtin_popcount((unsigned)x); }
Inline int Popcnt(unsigned x) { return __builtin_popcount(x); }
Inline int Popcnt(int64_t x) { return __builtin_popcountll((uint64_t)x); }
Inline int Popcnt(uint64_t x) { return __builtin_popcountll(x); }
Inline int Log2(int x) { return 31 - __builtin_clz((unsigned)x | 1); }
Inline int Log2(unsigned x) { return 31 - __builtin_clz(x | 1); }
Inline int Log2(int64_t x) { return 63 - __builtin_clzll((uint64_t)x | 1); }
Inline int Log2(uint64_t x) { return 63 - __builtin_clzll(x | 1); }

#define MULTI_TEST_CASES
class FastIS{static constexpr size_t P=1<<20;char r[P];char*u,*G;FILE*I;public:FastIS(FILE*Y=stdin):u(nullptr),G(nullptr),I(Y){}__attribute__((always_inline))inline int get(){return(u==G)&&(G=(u=r)+fread(r,1,P,I),u==G)?EOF:*u++;}template<typename A,typename=typename std::enable_if<std::is_arithmetic<A>::value>::type>FastIS&operator>>(A&j){if _GLIBCXX17_CONSTEXPR(std::is_same<A,bool>::value){int a=get();while(~a&&a!=(char)(48)&&a!=(char)(49))a=get();j=static_cast<bool>(a&1);}else if _GLIBCXX17_CONSTEXPR(std::is_same<A,char>::value){j=get();while(j==(char)(32)||j==(char)(13)||j==(char)(10))j=get();}else if _GLIBCXX17_CONSTEXPR(std::is_integral<A>::value){if _GLIBCXX17_CONSTEXPR(std::is_unsigned<A>::value){int a=get();while(~a&&(a<(char)(48)||a>(char)(57)))a=get();for(j=0;a>=(char)(48)&&a<=(char)(57);a=get())j=(j<<3)+(j<<1)+(a^(char)(48));}else{int a=get();bool m=false;while(~a&&(a<(char)(48)||a>(char)(57))&&(a^(char)(45)))a=get();if(a==(char)(45))a=get(),m=true;for(j=0;a>=(char)(48)&&a<=(char)(57);a=get())j=(j<<3)+(j<<1)+(a^(char)(48));j=m?-j:j;}}else if _GLIBCXX17_CONSTEXPR(std::is_floating_point<A>::value){int a=get();bool m=false;while(~a&&(a<(char)(48)||a>(char)(57))&&(a^(char)(45)))a=get();if(a==(char)(45))a=get(),m=true;using E=typename std::conditional<std::is_same<A,float>::value,std::uint32_t,std::uint64_t>::type;E Q;for(Q=0;a>=(char)(48)&&a<=(char)(57);a=get())Q=(Q<<3)+(Q<<1)+(a^(char)(48));j=Q;if(a==(char)(46)){E a=1;for(a=get();a>=(char)(48)&&a<=(char)(57);a=get())j+=(a^(char)(48))*(1./(a*=10));}if(m)j=-j;}else throw;return*this;}FastIS&operator>>(char*i){int a=get();while(~a&&(a==(char)(32)||a==(char)(13)||a==(char)(10)))a=get();while(~a&&(a!=(char)(32)&&a!=(char)(13)&&a!=(char)(10)))*i++=a,a=get();return*this;}}fin;class FastOS{static constexpr size_t P=1<<20;static constexpr double x=1e-9;char r[P],*b;const char*l;FILE*w;int o;public:FastOS(FILE*Y=stdout):b(r),l(r+P),w(Y),o(-1){setvbuf(w,nullptr,_IONBF,0);}~FastOS(){flush();fclose(w);}FastOS&flush(){fwrite(r,b-r,1,w);return*this;}FastOS&put(int g){*b++=g;if(b==l)fwrite(r,P,1,w),b=r;return*this;}FastOS&setprecision(const int&t){o=t;return*this;}template<typename a,typename=typename std::enable_if<std::is_arithmetic<a>::value>::type>FastOS&operator<<(a j){if _GLIBCXX17_CONSTEXPR(std::is_same<a,char>::value)put(j);else if _GLIBCXX17_CONSTEXPR(std::is_integral<a>::value){if _GLIBCXX17_CONSTEXPR(std::is_signed<a>::value)if(j<0)put((char)(45)),j=-j;static uint8_t i[40];i[*i=1]=j%10;while(j/=10)i[++(*i)]=j%10;while(*i)put(i[(*i)--]|(char)(48));}else if _GLIBCXX17_CONSTEXPR(std::is_floating_point<a>::value){if(j<0)put((char)(45)),j=-j;using E=typename std::conditional<std::is_same<a,float>::value,std::uint32_t,std::uint64_t>::type;E v=static_cast<E>(j);a B=j-v;*this<<v;if(o>0||(o<0&&B>x)){put((char)(46));for(int R=0;(o>0&&R<o)||(o<0&&B>x);R++){int X=static_cast<int>(B*=10);put(X|(char)(48));B-=X;}}}else throw;return*this;}FastOS&operator<<(char*i){while(*i)put(*i++);return*this;}}fout;

#ifdef DEBUG
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug(...) (void(0))
#endif

constexpr int N = 200005;
int a[N];
namespace BruteForce {
inline void solve(int n, int k) {
    cout << typeid(n).hash_code() << '\n';
    int loops = 0;
    long long cnt = 0;
    debug("Begin new cases with n = %d\n", n);
    auto arrLogger = [n, k](int p = 0) {
        debug("\t%c", (p == k ? '[' : ' '));
        For(i, 1, n) debug("%2d%c ", a[i],
                           i == p - k ? '['
                           : i == p   ? ']'
                                      : ' ');
        debug("\n");
    };
    arrLogger();
    while (!is_sorted(a + 1, a + 1 + n)) {
        debug("Loop %d:\n", ++loops);
        For(i, k, n) if (!is_sorted(a + 1 + i - k, a + 1 + i)) {
            sort(a + 1 + i - k, a + 1 + i);
            arrLogger(i);
            ++cnt;
        }
    }
    fout << cnt << '\n';
}
} // namespace BruteForce

namespace N2BruteForce {
int r[N], dat[N], tot;
inline void solve(int n, int k) {
    debug("-----------------\n");
    int cnt = 0;
    while (!is_sorted(a + 1, a + 1 + n)) {
        map<int, int> st;
        auto insertAfter = [&st](int p, int x) {
            dat[++tot] = x;
            r[tot] = r[p];
            r[p] = tot;
            st[x] = tot;
        };
        if (!is_sorted(a + 1, a + 1 + k))
            std::sort(a + 1, a + 1 + k), ++cnt;
        For(i, 2, k) r[i - 1] = i, dat[i - 1] = a[i], st[a[i]] = i - 1;
        r[0] = 1, r[k - 1] = 0, tot = k - 1;
        For(i, k + 1, n) {
            if (st.begin()->first > a[i]) { // a_i 放在最前面
                a[i - k + 1] = a[i], ++cnt;
            } else {
                if (st.rbegin()->first > a[i])
                    ++cnt;
                insertAfter(prev(st.lower_bound(a[i]))->second, a[i]);
                st.erase(dat[r[0]]);
                a[i - k + 1] = dat[r[0]]; // 放第一个
                r[0] = r[r[0]];           // 删掉第一个
            }
             debug("[%d] ", cnt); For(i, 1, n) debug("%d%c", a[i], i==n?'\n':' ');
        }
        int ptr = r[0];
        For(i, n - k + 2, n) a[i] = dat[ptr], ptr = r[ptr];
        For(i, 1, n) debug("%d%c", a[i], i==n?'\n':' ');
    }
    fout << cnt << '\n';
}
} // namespace N2BruteForce
namespace BIT {
int tr[N], n;
inline void init(int _n) {
    memset(tr, 0, sizeof tr);
    n = _n;
}
inline void upd(int p, int x) {
    for (; p <= n; p += p & -p)
        tr[p] += x;
}
inline int sum(int p) {
    int res = 0;
    for (; p > 0; p -= p & -p)
        res += tr[p];
    return res;
}
} // namespace BIT
namespace K2InvPair {
inline void solve(int n) {
    BIT::init(n);
    long long ans = 0;
    For(i, 1, n) {
        ans += BIT::sum(n) - BIT::sum(a[i]);
        BIT::upd(a[i], 1);
    }
    fout << ans << '\n';
}
} // namespace K2InvPair

namespace Std {
bool vis[N];
inline void solve(int n, int k) {
    memset(vis, 0, sizeof(bool) * (n+1));
    BIT::init(n);
    long long ans = 0;
    For(i, 1, n) {
        int tp = (BIT::sum(n) - BIT::sum(a[i]) + (k-1) - 1) / (k-1);
        if (i - (tp - 1) * (k - 1) <= k) {
            if (vis[tp]) ans += tp - 1;
            else vis[tp] = true, ans += tp;
        }
        else ans += tp;
        BIT::upd(a[i], 1);
    }
    fout << ans << '\n';
}
} // namespace Std

inline void solveSingleTestCase() {
    int n, k;
    fin >> n >> k;
    For(i, 1, n) fin >> a[i];
    if (n <= 200)
        BruteForce::solve(n, k);
    else if (n <= 2000)
        N2BruteForce::solve(n, k);
    else if (k == 2)
        K2InvPair::solve(n);
    else
        Std::solve(n, k);
}
int main() {
    freopen("maze.in", "r", stdin);
    freopen("maze.out", "w", stdout);
#ifdef DEBUG
    freopen("maze.txt", "w", stderr);
#endif
    // cin.tie(nullptr) -> sync_with_stdio(false);
    int testCases = 1;
#ifdef MULTI_TEST_CASES
    fin >> testCases;
#endif
    while (testCases--)
        solveSingleTestCase();
    return 0;
}