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
template<typename b>concept IntegerWithI128=std::integral<b>||std::same_as<b,__int128_t>||std::same_as<b,__uint128_t>;class FastIS{static constexpr size_t d=1<<20;char e[d];char*f=nullptr,*g=nullptr;FILE*h;public:inline FastIS(FILE*i=stdin):h(i){}[[gnu::always_inline]]inline char get(){if(f==g){f=e;g=e+fread(e,1,d,h);if(f==g)return EOF;}return*f++;}inline FastIS&operator>>(bool&k){int l=get();while(~l&&l!='0'&&l!='1')l=get();k=(l=='1');return*this;}inline FastIS&operator>>(char&m){m=get();while(m==' '||m=='\r'||m=='\n'||m=='\t')m=get();return*this;}template<IntegerWithI128 n>inline FastIS&operator>>(n&o){int p=get();bool q=false;while(~p&&(p<'0'||p>'9')&&p!='-')p=get();if constexpr(std::is_signed_v<n>||std::same_as<n,__int128_t>){if(p=='-'){q=true;p=get();}}o=0;while(p>='0'&&p<='9'){o=(o<<3)+(o<<1)+(p^'0');p=get();}if(q)o=-o;return*this;}template<std::floating_point r>inline FastIS&operator>>(r&s){int t=get();bool u=false;while(~t&&(t<'0'||t>'9')&&t!='-')t=get();if(t=='-'){u=true;t=get();}__uint128_t v=0;while(t>='0'&&t<='9'){v=(v<<3)+(v<<1)+(t^'0');t=get();}s=static_cast<r>(v);if(t=='.'){r w=1.0;for(t=get();t>='0'&&t<='9';t=get()){s+=(t^'0')*(w/=10.0);}}if(u)s=-s;return*this;}inline FastIS&operator>>(char*x){int y=get();while(~y&&(y<=' '))y=get();while(~y&&(y>' '))*x++=y,y=get();*x='\0';return*this;}}fin;class FastOS{static constexpr size_t d=1<<20;char e[d],*f=e;FILE*g;int h=-1;public:inline FastOS(FILE*i=stdout):g(i){setvbuf(g,nullptr,_IONBF,0);}inline~FastOS(){flush();}inline void flush(){fwrite(e,1,f-e,g);f=e;}[[gnu::always_inline]]inline void put(char l){if(f==e+d)flush();*f++=l;}inline FastOS&setprecision(int n){h=n;return*this;}inline FastOS&operator<<(char o){put(o);return*this;}inline FastOS&operator<<(const char*p){while(*p)put(*p++);return*this;}FastOS&operator<<(const std::string&q){for(char r:q)put(r);return*this;}template<IntegerWithI128 s>inline FastOS&operator<<(s t){if(t==0){put('0');return*this;}s u=t;if constexpr(std::is_signed_v<s>||std::same_as<s,__int128_t>){if(u<0){put('-');u=-u;}}static char v[64];int w=0;while(u){v[w++]=static_cast<char>(u%10)^'0';u/=10;}while(w)put(v[--w]);return*this;}template<std::floating_point x>inline FastOS&operator<<(x y){if(y<0){put('-');y=-y;}__uint128_t z=static_cast<__uint128_t>(y);*this<<z;x A=y-static_cast<x>(z);if(h>=0||A>1e-12){put('.');int B=(h>=0)?h:6;while(B--){A*=10;int C=static_cast<int>(A);put(C^'0');A-=C;}}return*this;}}fout;
FastOS ferr(stderr);
constexpr int N = 500005; // TO BE MODIFIED!!!
vector<int> g[N];
int fa[N], sf[N], d[N], ord[N];
int64_t sum[N];
using i128 = __int128;
constexpr i128 TOP = 1e18;
i128 exgcd(i128 a, i128 b, i128& x, i128& y) {
    if (b == 0) { x = 1, y = 0; return a; }
    auto gcd = exgcd(b, a%b, y, x);
    y -= a/b*x;
    return gcd;
}
void dfs(int u, int& real, i128 m, i128 lcm) {
    if (!d[u]) { real = u; return; }
    if (~lcm || lcm > TOP) { lcm = -1; goto bf; }
    if (i128 gcd = __gcd(lcm, (i128)d[u]), nlcm = lcm / gcd * d[u]; nlcm != lcm) {
        real = u;
        for (int& v : g[u]) {
            // x \equiv m     (mod lcm)
            // x \equiv sf[v] (mod d[u])
            if ((m - sf[v]) % gcd) continue;
            i128 x, y;
            exgcd(m, sf[v], x, y);
            i128 nm = ((sf[v] * y + d[u]) % nlcm + nlcm) * ((m - sf[v]) / gcd) % nlcm;
            ferr << u << "-->" << v << ": " << nm << ' ' << nlcm << '\n';
            dfs(v, v, nm, nlcm);
        }
        return;
    }
bf: dfs(g[u][(m + sum[u]) % d[u]], real, m, lcm);
}
inline void solveSingleTestCase() {
    int n, q;
    fin >> n >> q;
    For (i, 1, n) g[i].clear();
    memset(d+1, 0, sizeof(*d) * n);
    For (i, 2, n) {
        fin >> fa[i];
        ++d[fa[i]];
        ord[i] = g[fa[i]].size();
        g[fa[i]].push_back(i);
    }
    For (i, 2, n) {
        fin >> sum[i];
        sum[i] += sum[fa[i]];
        sf[i] = (sum[i] - ord[i] + d[fa[i]]) % d[fa[i]];
    }
    { int fku; dfs(1, fku, 1, 1); }
    Rep (q) {
        int64_t m;
        fin >> m;
        int u = 1;
        // ferr << "Inquire " << m << ": ";
        while (d[u]) {
            // ferr << '[' << u << ']' << ((m + sum[u]) % d[u]) << "th ";
            u = g[u][(m + sum[u]) % d[u]];
        }
        // ferr << '\n';
        fout << u << ' ';
    }
    fout << '\n';
}
int main() {
    int testCases = 1;
#ifdef MULTI_TEST_CASES
    fin >> testCases;
#endif
    while (testCases--) solveSingleTestCase();
    return 0;
}